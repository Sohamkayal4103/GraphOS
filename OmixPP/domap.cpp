#include "domap.h"
#include <vector>
#include <unordered_map>
#include "helper.h"
#include <iostream>
using namespace std;

class stash;

domap::domap(vector<vector<int>> &givenKeyValues, int Z)
{
  this->Z = Z;
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
  // set treeHeight and N
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
      // fill all the slots with dummy nodes
      buckets[i][j] = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
    }
  }
  mainStash = stash(rootHeight);
  ////cout << "Root height: " << rootHeight << endl;
  this->initialize();
}

void domap::initialize()
{
  ////cout << "Initializing the tree" << endl;
  ////cout << "Root height: " << rootHeight << endl;
  // iterate through and create a vector of nodes
  vector<node> tempNodes;
  for (auto it = keyValues.begin(); it != keyValues.end(); it++)
  {
    tempNodes.push_back(node(it->first, it->second, 0, 0, 0, "-1", "-1", -1, -1, -1, 0, 0));
  }

  // sort the nodes
  helper h = helper();
  auto cmpKey = [](const node &a, const node &b) -> bool
  {
    return a.key < b.key;
  };
  h.bitonicSort(tempNodes, 0, static_cast<int>(tempNodes.size()), true, cmpKey);
  // ////cout << "sorted nodes: ";
  // for (int i = 0; i < (int)tempNodes.size(); i++)
  // {
  //   ////cout << tempNodes[i].key << " ";
  // }
  // ////cout << endl;

  // normal sort
  sort(tempNodes.begin(), tempNodes.end(), cmpKey);
  // ////cout << "sorted nodes: ";
  // for (int i = 0; i < (int)tempNodes.size(); i++)
  // {
  //   ////cout << tempNodes[i].key << " ";
  // }
  // ////cout << endl;

  // create AVL tree
  pair<int, pair<int, string>> rootData = createAVLTree(0, (int)keyValues.size() - 1, tempNodes);

  // rootHeight = rootData.first;
  rootPos = rootData.second.first;
  rootKey = rootData.second.second;
  ////cout << "Root key: " << rootKey << endl;
  ////cout << "Root pos: " << rootPos << endl;
  ////cout << "Initialization complete" << endl;

  initialEviction();

  ////cout << endl;
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
    // generate random position for the node
    helper h = helper();
    int pos = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);

    // setting data for the node
    tempNodes[l].pos = pos;
    tempNodes[l].height = 0;
    tempNodes[l].leftPos = -1;
    tempNodes[l].rightPos = -1;
    tempNodes[l].leftKey = "-1";
    tempNodes[l].rightKey = "-1";
    tempNodes[l].isDummy = 0;
    tempNodes[l].leftHeight = -1;
    tempNodes[l].rightHeight = -1;

    // for (int i = 0; i < Z; i++)
    // {
    //   if (buckets[pos][i].isDummy)
    //   {
    //     // insert the node in the bucket
    //     // just for debugging
    //     // ////cout << "Inserting node with key: " << tempNodes[l].key << " at pos: " << pos << endl;
    //     buckets[pos][i] = tempNodes[l];
    //     break;
    //   }
    // }
    // instead of inserting it in the tree store it in stash
    mainStash.nodes.push_back(tempNodes[l]);
    return {0, {tempNodes[l].pos, tempNodes[l].key}};
  }
  int mid = l + (r - l) / 2;
  pair<int, pair<int, string>> leftData = createAVLTree(l, mid - 1, tempNodes);
  pair<int, pair<int, string>> rightData = createAVLTree(mid + 1, r, tempNodes);
  int leftHeight = leftData.first;
  int rightHeight = rightData.first;
  int height = max(leftHeight, rightHeight) + 1;

  // setting data for the node
  tempNodes[mid].height = height;
  tempNodes[mid].leftPos = leftData.second.first;
  tempNodes[mid].rightPos = rightData.second.first;
  tempNodes[mid].leftKey = leftData.second.second;
  tempNodes[mid].rightKey = rightData.second.second;

  // generate random position for the node
  helper h = helper();
  int pos = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);
  tempNodes[mid].pos = pos;
  for (int i = 0; i < Z; i++)
  {
    if (buckets[pos][i].isDummy)
    {
      // insert the node in the bucket
      // just for debugging
      // ////cout << "Insertiing node with key: " << tempNodes[mid].key << " at pos: " << pos << endl;
      buckets[pos][i] = tempNodes[mid];
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
      if (buckets[bucketId][i].isDummy)
        continue;
      mainStash.nodes.push_back(buckets[bucketId][i]);
      // replacing with dummy node
      buckets[bucketId][i] = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
    }
    bucketId /= 2;
  }
}

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

node domap::evictStash(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal)
{
  node retNode = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
  for (int i = 0; i < (int)mainStash.nodes.size(); i++)
  {
    if (mainStash.nodes[i].key == currKey)
    {
      if (currKey == key)
      {
        // update the value for update operation
        if (op == 1)
        {
          mainStash.nodes[i].value = newVal;
        }
      }
      // find intersecting bucket between currPos and newPos
      int intersectingBucket = findIntersectingBucket(currPos, newPos);
      // ////cout << "node: " << mainStash.nodes[i].key << " currPos " << currPos << " newPos: " << newPos << " intersectingBucket: " << intersectingBucket << endl;
      mainStash.nodes[i].pos = newPos;
      mainStash.nodes[i].level = log2(intersectingBucket);
      retNode = mainStash.nodes[i];
    }
    else
    {
      // find intersecting bucket between currPos and newPos
      int currNodePos = mainStash.nodes[i].pos;
      int intersectingBucket = findIntersectingBucket(currNodePos, currPos);
      // ////cout << "node: " << mainStash.nodes[i].key << " currNodePos " << currNodePos << " newPos: " << currPos << " intersectingBucket: " << intersectingBucket << endl;
      mainStash.nodes[i].pos = newPos;
      mainStash.nodes[i].level = log2(intersectingBucket);
    }
  }
  // ////cout << endl;

  // sort by levels
  auto cmpLevel = [](const node &a, const node &b) -> bool
  {
    return a.level > b.level;
  };
  sort(mainStash.nodes.begin(), mainStash.nodes.end(), cmpLevel);
  // ////cout << "sorted stash: ";
  // for (int i = 0; i < (int)mainStash.nodes.size(); i++)
  // {
  //   ////cout << mainStash.nodes[i].key << " ";
  // }
  // ////cout << endl;
  // start from bucketId = currPos
  int bucketId = currPos;
  int currLevel = log2(bucketId);
  int p1 = 0;
  // ////cout << "Now inserting in the tree" << endl;
  while (bucketId > 0)
  {
    // ////cout << bucketId << " ";
    // set the bucketId and sort again based on bucketId
    currLevel = log2(bucketId);
    if (p1 < (int)mainStash.nodes.size() && currLevel <= mainStash.nodes[p1].level)
    {
      // ////cout << "followed this" << endl;
      for (int i = 0; i < Z; i++)
      {
        if (buckets[bucketId][i].isDummy)
        {
          buckets[bucketId][i] = mainStash.nodes[p1];
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
  // empty the stash
  // mainStash.nodes.clear();
  // make size 0
  // mainStash.nodes.resize(0);
  return retNode;
}

node domap::access(string &key, string &currKey, int bucketId, int newPos, int op, pair<int, int> newVal)
{
  ////cout << "Accessing key: " << currKey << " to reach " << key << " at pos " << bucketId << endl;
  // if (ct > 20)
  // {
  //   ////cout << "returning dummy for now" << endl;
  //   return node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1);
  // }
  // just for debugging
  // ////cout << "Accessing key: " << currKey << " from bucket: " << bucketId << endl;

  // fetch the path
  // ////cout << "Size of stash: " << mainStash.nodes.size() << endl;
  fetchPath(bucketId);
  node currNode = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
  // ////cout << "State of the stash:" << endl;
  for (int i = 0; i < (int)mainStash.nodes.size(); i++)
  {
    // ////cout << "node key: " << mainStash.nodes[i].key << " pos: " << mainStash.nodes[i].pos << endl;
    if (mainStash.nodes[i].key == currKey)
    {
      currNode = mainStash.nodes[i];
    }
  }
  // ////cout << "End of state of the stash" << endl;

  // generate new Position between 2^h and 2^(h+1) - 1
  helper h = helper();
  int newPosChild = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);

  // evict the stash
  node tempNode = evictStash(key, currKey, newPos, bucketId, op, newVal);
  // ////cout << "tempNode.key: " << tempNode.key << endl;

  node updatedChildNode;
  // decide whether to go left or right or return from here
  // ////cout << "Current node key: " << currNode.key << " " << currKey << endl;
  if (currNode.key == "-1")
  {
    // ////cout << "Node not found" << endl;
    // just for debugging
    // ////cout << "Returning dummy node" << endl;
    ////cout << "Printing Stash for debugging" << endl;
    ////cout << "Element looking for: " << currKey << " to finally find " << key << endl;
    for (int i = 0; i < (int)mainStash.nodes.size(); i++)
    {
      ////cout << "Node: " << mainStash.nodes[i].key << endl;
    }
    ////cout << "End of stash" << endl;
    ////cout << "Returning dummy node" << endl;
    return currNode;
  }
  else if (currNode.key == key && currNode.isDummy == 0)
  {
    // ////cout << "Node found" << endl;
    // ////cout << "Returning node" << " " << tempNode.key << " " << tempNode.pos << " " << tempNode.value.first << "," << tempNode.value.second << endl;
    ////cout << "And found the node: " << currNode.key << endl;
    return currNode;
  }
  else if (currNode.key < key && currNode.rightKey != "-1")
  {
    // ////cout << "Going right" << endl;
    updatedChildNode = access(key, currNode.rightKey, currNode.rightPos, newPosChild, op, newVal);
    currNode.rightPos = newPosChild;
  }
  else if (currNode.key > key && currNode.leftKey != "-1")
  {
    // ////cout << "Going left" << endl;
    updatedChildNode = access(key, currNode.leftKey, currNode.leftPos, newPosChild, op, newVal);
    currNode.leftPos = newPosChild;
  }
  return updatedChildNode;
}

pair<int, int> domap::accessNode(string &key, int op, pair<int, int> newVal)
{
  helper h = helper();
  int newPos = h.generateRandomNumber(pow(2, rootHeight - 1), pow(2, rootHeight) - 1);
  // ////cout << "Access trace begins" << endl;
  node ansNode = access(key, rootKey, rootPos, newPos, op, newVal);
  rootPos = newPos;
  // ////cout << "Access trace ends" << endl;
  // ////cout << endl;
  // ////cout << "new root key: " << rootKey << endl;
  // ////cout << "new root pos: " << rootPos << endl;

  // ////cout << endl;
  // ////cout << endl;
  // ////cout << endl;
  // ////cout << endl;
  // printState();
  // ////cout << endl;
  // ////cout << endl;
  // ////cout << endl;

  return ansNode.value;
}

// just for debugging
void domap::printState()
{
  // ////cout << "State of the tree:" << endl;
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    // ////cout << "Bucket: " << i << endl;
    for (int j = 0; j < Z; j++)
    {
      if (buckets[i][j].isDummy)
        continue;
      // ////cout << "Node: " << j << endl;
      buckets[i][j].printNode();
      ////cout << endl;
    }
    // ////cout << endl;
  }
  //////cout << "End of state of the tree" << endl;
}

void domap::addEdge(int u, int v, int w)
{
  // add edge to the graph

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

  // update the tree
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
      // fill all the slots with dummy nodes
      buckets[i][j] = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
    }
  }
  mainStash = stash(rootHeight);
  ////cout << "Root height: " << rootHeight << endl;
  this->initialize();
}

void domap::addEdgeByString(string k1, pair<int, int> val)
{
  keyValues[k1] = val;

  // update the tree
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
      // fill all the slots with dummy nodes
      buckets[i][j] = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
    }
  }
  mainStash = stash(rootHeight);
  ////cout << "Root height: " << rootHeight << endl;
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
        if (buckets[bucketId][j].isDummy)
        {
          buckets[bucketId][j] = mainStash.nodes[i];
          buckets[bucketId][j].level = log2(bucketId);
          // mainStash.nodes[i] = node("-1", {0, 0}, 0, 0, 0, "-1", "-1", -1, -1, -1, 1, 0);
          ////cout << "Placed node: " << mainStash.nodes[i].key << " at bucketId: " << bucketId << endl;
          placed = 1;
          break;
        }
      }
      if (placed == 0)
      {
        // move to parent bucket
        bucketId /= 2;
        if (bucketId == 0)
        {
          ////cout << "Not able to place node: " << mainStash.nodes[i].key << endl;
        }
      }
    }
  }
  mainStash.nodes.clear();
  mainStash.nodes.resize(0);
  ////cout << "Initial eviction complete" << endl;
}