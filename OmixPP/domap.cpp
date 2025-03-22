#include "domap.h"
#include <vector>
#include <unordered_map>
#include "helper.h"
#include <iostream>
#include <cmath>
#include <sstream>

// Boost Serialization headers
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// Crypto++ headers
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

using namespace std;
using namespace CryptoPP;

namespace
{
  // --- Global Crypto Setup and Helper Functions ---

  // Global key and IV (for demonstration; in practice, manage these securely)
  SecByteBlock globalKey(AES::DEFAULT_KEYLENGTH);
  SecByteBlock globalIV(AES::BLOCKSIZE);

  void initializeCrypto()
  {
    AutoSeededRandomPool prng;
    prng.GenerateBlock(globalKey, globalKey.size());
    prng.GenerateBlock(globalIV, globalIV.size());
  }

  // Serializes and encrypts a node
  string encryptAndSerialize(const node &n)
  {
    stringstream ss;
    boost::archive::text_oarchive oa(ss);
    oa << n;
    string plainText = ss.str();

    string cipherText;
    try
    {
      CBC_Mode<AES>::Encryption encryption;
      encryption.SetKeyWithIV(globalKey, globalKey.size(), globalIV, globalIV.size());
      StringSource(plainText, true,
                   new StreamTransformationFilter(encryption,
                                                  new StringSink(cipherText)));
    }
    catch (const Exception &e)
    {
      cerr << "Encryption error: " << e.what() << endl;
    }
    return cipherText;
  }

  // Decrypts and deserializes a node
  node decryptAndDeserialize(const string &cipherText)
  {
    string recovered;
    try
    {
      CBC_Mode<AES>::Decryption decryption;
      decryption.SetKeyWithIV(globalKey, globalKey.size(), globalIV, globalIV.size());
      StringSource(cipherText, true,
                   new StreamTransformationFilter(decryption,
                                                  new StringSink(recovered)));
    }
    catch (const Exception &e)
    {
      cerr << "Decryption error: " << e.what() << endl;
    }
    node n;
    stringstream ss(recovered);
    boost::archive::text_iarchive ia(ss);
    ia >> n;
    return n;
  }

  // Returns the encrypted form of a dummy node (dummy: key "-1" and isDummy==1)
  string getEncryptedDummy()
  {
    node dummy("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
    return encryptAndSerialize(dummy);
  }

  // For checking if a bucket slot is empty (dummy) without decryption
  static string encryptedDummy = getEncryptedDummy();

  // Helper function to compute the intersecting bucket of two positions.
  int findIntersectingBucket(int currBucketId, int newBucketId)
  {
    int currBucketIdCopy = currBucketId;
    int newBucketIdCopy = newBucketId;
    while (currBucketIdCopy != newBucketIdCopy)
    {
      currBucketIdCopy /= 2;
      newBucketIdCopy /= 2;
    }
    return max(currBucketIdCopy, 1);
  }

} // end anonymous namespace

// --- domap Member Function Implementations ---

domap::domap(vector<vector<int>> &givenKeyValues, int Z)
{
  // Initialize the crypto materials.
  initializeCrypto();
  this->Z = Z;
  // Process given key–values and update the mapping.
  for (int i = 0; i < (int)givenKeyValues.size(); i++)
  {
    int u = givenKeyValues[i][0];
    int v = givenKeyValues[i][1];
    int w = givenKeyValues[i][2];

    string k1 = "V" + to_string(u);
    string k2 = "V" + to_string(v);
    if (keyValues.find(k1) == keyValues.end())
      keyValues[k1] = {0, 0};
    if (keyValues.find(k2) == keyValues.end())
      keyValues[k2] = {0, 0};
    keyValues[k1].second++;
    keyValues[k2].first++;

    string k3 = "Eout" + to_string(u) + "_" + to_string(keyValues[k1].second);
    string k4 = "Ein" + to_string(v) + "_" + to_string(keyValues[k2].first);
    keyValues[k3] = {v, w};
    keyValues[k4] = {u, w};
  }
  // Set tree height and number of leaves.
  rootHeight = ceil(log2(keyValues.size()));
  int N = 1;
  while (N < (int)keyValues.size())
    N *= 2;
  numLeaves = N;
  // Resize buckets (each bucket holds encrypted nodes)
  buckets.resize(pow(2, rootHeight));
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    buckets[i].resize(Z);
  }
  // Fill all slots with the encrypted dummy node.
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    for (int j = 0; j < Z; j++)
    {
      buckets[i][j] = encryptedDummy;
    }
  }
  mainStash = stash(rootHeight);
  this->initialize();
}

void domap::initialize()
{
  // Create a vector of plaintext nodes from keyValues.
  vector<node> tempNodes;
  for (auto it = keyValues.begin(); it != keyValues.end(); it++)
  {
    tempNodes.push_back(node(it->first, it->second, 0, 0, 0, "-1", "-1", -1, -1, -1, 0, 0));
  }

  // Sort the nodes.
  helper h;
  auto cmpKey = [](const node &a, const node &b) -> bool
  { return a.key < b.key; };
  h.bitonicSort(tempNodes, 0, static_cast<int>(tempNodes.size()), true, cmpKey);
  sort(tempNodes.begin(), tempNodes.end(), cmpKey);

  // Create the AVL tree; note that for nodes that are inserted into buckets,
  // we do NOT decrypt the bucket—they are written in encrypted form.
  pair<int, pair<int, string>> rootData = createAVLTree(0, (int)keyValues.size() - 1, tempNodes);
  rootPos = rootData.second.first;
  rootKey = rootData.second.second;

  // After building the tree (or parts of it), perform an initial eviction from the stash.
  initialEviction();
}

pair<int, pair<int, string>> domap::createAVLTree(int l, int r, vector<node> &tempNodes)
{
  int n = r - l + 1;
  if (n <= 0)
  {
    return {-1, {-1, "-1"}};
  }
  if (n == 1)
  {
    helper h;
    int pos = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);

    // Set node parameters.
    tempNodes[l].pos = pos;
    tempNodes[l].height = 0;
    tempNodes[l].leftPos = -1;
    tempNodes[l].rightPos = -1;
    tempNodes[l].leftKey = "-1";
    tempNodes[l].rightKey = "-1";
    tempNodes[l].isDummy = 0;
    tempNodes[l].leftHeight = -1;
    tempNodes[l].rightHeight = -1;

    // Instead of inserting directly into the tree bucket, store it in the stash.
    mainStash.nodes.push_back(tempNodes[l]);
    return {0, {tempNodes[l].pos, tempNodes[l].key}};
  }
  int mid = l + (r - l) / 2;
  pair<int, pair<int, string>> leftData = createAVLTree(l, mid - 1, tempNodes);
  pair<int, pair<int, string>> rightData = createAVLTree(mid + 1, r, tempNodes);
  int leftHeight = leftData.first;
  int rightHeight = rightData.first;
  int height = max(leftHeight, rightHeight) + 1;

  // Set current node's parameters.
  tempNodes[mid].height = height;
  tempNodes[mid].leftPos = leftData.second.first;
  tempNodes[mid].rightPos = rightData.second.first;
  tempNodes[mid].leftKey = leftData.second.second;
  tempNodes[mid].rightKey = rightData.second.second;

  helper h;
  int pos = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);
  tempNodes[mid].pos = pos;
  // Insert the node into the appropriate bucket.
  // Since buckets store encrypted strings, we check for an available slot by comparing
  // against the known encrypted dummy value (without decrypting).
  for (int i = 0; i < Z; i++)
  {
    if (buckets[pos][i] == encryptedDummy)
    {
      buckets[pos][i] = encryptAndSerialize(tempNodes[mid]);
      break;
    }
  }
  return {height, {tempNodes[mid].pos, tempNodes[mid].key}};
}

void domap::fetchPath(int bucketId)
{
  while (bucketId > 0)
  {
    for (int i = 0; i < Z; i++)
    {
      // Check if the slot contains the dummy ciphertext.
      if (buckets[bucketId][i] == encryptedDummy)
      {
        continue; // Skip decryption since this is a dummy node.
      }

      // Otherwise, decrypt and deserialize.
      node fetched = decryptAndDeserialize(buckets[bucketId][i]);
      if (fetched.isDummy)
      {
        // In case decryption returns a dummy node, ensure the slot is set back.
        buckets[bucketId][i] = encryptedDummy;
        continue;
      }

      mainStash.nodes.push_back(fetched);
      // Replace the bucket slot with the encrypted dummy value.
      buckets[bucketId][i] = encryptedDummy;
    }
    bucketId /= 2;
  }
}

node domap::evictStash(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal)
{
  node retNode("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
  for (int i = 0; i < (int)mainStash.nodes.size(); i++)
  {
    if (mainStash.nodes[i].key == currKey)
    {
      if (currKey == key)
      {
        if (op == 1)
          mainStash.nodes[i].value = newVal;
      }
      int intersectingBucket = findIntersectingBucket(currPos, newPos);
      mainStash.nodes[i].pos = newPos;
      mainStash.nodes[i].level = log2(intersectingBucket);
      retNode = mainStash.nodes[i];
    }
    else
    {
      int currNodePos = mainStash.nodes[i].pos;
      int intersectingBucket = findIntersectingBucket(currNodePos, currPos);
      mainStash.nodes[i].pos = newPos;
      mainStash.nodes[i].level = log2(intersectingBucket);
    }
  }
  auto cmpLevel = [](const node &a, const node &b) -> bool
  {
    return a.level > b.level;
  };
  helper h;
  h.bitonicSort(mainStash.nodes, 0, static_cast<int>(mainStash.nodes.size()), true, cmpLevel);
  sort(mainStash.nodes.begin(), mainStash.nodes.end(), cmpLevel);

  int bucketId = currPos;
  int currLevel = log2(bucketId);
  int p1 = 0;
  while (bucketId > 0)
  {
    currLevel = log2(bucketId);
    if (p1 < (int)mainStash.nodes.size() && currLevel <= mainStash.nodes[p1].level)
    {
      for (int i = 0; i < Z; i++)
      {
        if (buckets[bucketId][i] == encryptedDummy)
        {
          buckets[bucketId][i] = encryptAndSerialize(mainStash.nodes[p1]);
          p1++;
          break;
        }
        else
        {
          bucketId /= 2;
        }
      }
    }
    else
    {
      bucketId /= 2;
    }
  }
  return retNode;
}

node domap::access(string &key, string &currKey, int bucketId, int newPos, int op, pair<int, int> newVal)
{
  // Fetch the full path into the stash (decryption happens in the stash only)
  fetchPath(bucketId);
  node currNode("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
  for (int i = 0; i < (int)mainStash.nodes.size(); i++)
  {
    if (mainStash.nodes[i].key == currKey)
      currNode = mainStash.nodes[i];
  }
  helper h;
  int newPosChild = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);
  node tempNode = evictStash(key, currKey, newPos, bucketId, op, newVal);
  node updatedChildNode;
  if (currNode.key == "-1")
  {
    return currNode;
  }
  else if (currNode.key == key && currNode.isDummy == 0)
  {
    return currNode;
  }
  else if (currNode.key < key && currNode.rightKey != "-1")
  {
    updatedChildNode = access(key, currNode.rightKey, currNode.rightPos, newPosChild, op, newVal);
    currNode.rightPos = newPosChild;
  }
  else if (currNode.key > key && currNode.leftKey != "-1")
  {
    updatedChildNode = access(key, currNode.leftKey, currNode.leftPos, newPosChild, op, newVal);
    currNode.leftPos = newPosChild;
  }
  return updatedChildNode;
}

pair<int, int> domap::accessNode(string &key, int op, pair<int, int> newVal)
{
  helper h;
  int newPos = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);
  node ansNode = access(key, rootKey, rootPos, newPos, op, newVal);
  rootPos = newPos;
  return ansNode.value;
}

void domap::printState()
{
  // For debugging, we avoid decrypting buckets here (per the criteria decryption happens in the stash).
  // We simply indicate which buckets hold non-dummy (encrypted) data.
  cout << "State of the tree (bucket-level):" << endl;
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    for (int j = 0; j < Z; j++)
    {
      if (buckets[i][j] != encryptedDummy)
        cout << "Bucket " << i << ", Slot " << j << " contains an encrypted node." << endl;
    }
  }
  cout << "\nStash contents:" << endl;
  for (auto &n : mainStash.nodes)
  {
    n.printNode();
    cout << endl;
  }
}

void domap::addEdge(int u, int v, int w)
{
  string k1 = "V" + to_string(u);
  string k2 = "V" + to_string(v);
  if (keyValues.find(k1) == keyValues.end())
    keyValues[k1] = {0, 0};
  if (keyValues.find(k2) == keyValues.end())
    keyValues[k2] = {0, 0};
  keyValues[k1].second++;
  keyValues[k2].first++;
  string k3 = "Eout" + to_string(u) + "_" + to_string(keyValues[k1].second);
  string k4 = "Ein" + to_string(v) + "_" + to_string(keyValues[k2].first);
  keyValues[k3] = {v, w};
  keyValues[k4] = {u, w};

  rootHeight = ceil(log2(keyValues.size()));
  int N = 1;
  while (N < (int)keyValues.size())
    N *= 2;
  numLeaves = N;
  buckets.resize(pow(2, rootHeight));
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    buckets[i].resize(Z);
  }
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    for (int j = 0; j < Z; j++)
    {
      buckets[i][j] = encryptedDummy;
    }
  }
  mainStash = stash(rootHeight);
  this->initialize();
}

void domap::addEdgeByString(string k1, pair<int, int> val)
{
  keyValues[k1] = val;

  int N = 1;
  while (N < (int)keyValues.size())
    N *= 2;
  numLeaves = N;
  buckets.resize(pow(2, rootHeight));
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    buckets[i].resize(Z);
  }
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    for (int j = 0; j < Z; j++)
    {
      buckets[i][j] = encryptedDummy;
    }
  }
  mainStash = stash(rootHeight);
  this->initialize();
}

bool domap::contains(string &key)
{
  pair<int, int> ans = accessNode(key, 0, {0, 0});
  if (ans.first == 0 && ans.second == 0)
    return false;
  return true;
}

void domap::initialEviction()
{
  for (int i = 0; i < (int)mainStash.nodes.size(); i++)
  {
    int placed = 0;
    int bucketId = mainStash.nodes[i].pos;
    while (placed == 0 && bucketId > 0)
    {
      for (int j = 0; j < Z; j++)
      {
        if (buckets[bucketId][j] == encryptedDummy)
        {
          node tmp = mainStash.nodes[i];
          tmp.level = log2(bucketId);
          buckets[bucketId][j] = encryptAndSerialize(tmp);
          placed = 1;
          break;
        }
      }
      if (placed == 0)
      {
        bucketId /= 2;
      }
    }
  }
  mainStash.nodes.clear();
  mainStash.nodes.resize(0);
}

node domap::naiveEviction(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal)
{
  // For simplicity, return a dummy node.
  return node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
}
