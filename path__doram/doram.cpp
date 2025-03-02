#include "doram.h"
#include <vector>
#include <unordered_map>
#include "helper.h"
#include <iostream>
using namespace std;

class Stash;

DORAM::DORAM(int maxNodes)
{
  cout << "Max nodes: " << maxNodes << endl;
  cout << "treeHeight: " << log2(maxNodes) << endl;
  cout << endl;
  treeHeight = ceil(log2(maxNodes));
  mainStash = new Stash(treeHeight);
  N = 1;
  while (N < maxNodes)
    N *= 2;
  numLeaves = N;
  treeHeight = (int)log2(N);
  Z = 4;
  buckets.resize(pow(2, treeHeight + 1));
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    buckets[i].resize(Z);
  }
  for (int i = 0; i < (int)buckets.size(); i++)
  {
    for (int j = 0; j < Z; j++)
    {
      // fill all the slots with dummy nodes
      buckets[i][j] = new Node(-1, 0, 1);
    }
  }
}

void DORAM::initialize(vector<vector<int>> &keyValues)
{
  cout << "Initializing DORAM" << endl;
  Stash *st = mainStash;
  mainStash = st;
  pair<int, int> rootData = createAVLTree(0, keyValues.size() - 1, keyValues, st);
  rootPos = rootData.first;
  rootKey = rootData.second;
  cout << "Root key: " << rootKey << endl;
  cout << "Root pos: " << rootPos << endl;
  initialEviction(st);
}

void DORAM::initialEviction(Stash *st)
{
  cout << "Initial eviction started:" << endl;
  // iterate through the stash and evict all the nodes
  for (int i = 0; i < (int)st->nodes.size(); i++)
  {
    int targetBucketId = st->nodes[i]->pos;
    while (targetBucketId > 0)
    {
      int placed = 0;
      for (int j = 0; j < Z; j++)
      {
        if (buckets[targetBucketId][j]->isDummy)
        {
          buckets[targetBucketId][j] = st->nodes[i];
          // cout << "Now: " << buckets[targetBucketId][j]->isDummy << endl;
          placed = 1;
          break;
        }
      }
      if (placed == 1)
      {
        cout << "Placed node with key: " << st->nodes[i]->key << " at pos: " << targetBucketId << endl;
        break;
      }
      targetBucketId /= 2;
    }
  }
  st->nodes.clear();
  cout << "Initial eviction completed" << endl;
}

pair<int, int> DORAM::createAVLTree(int l, int r, vector<vector<int>> &keyValues, Stash *st)
{
  if (l > r)
    return {-1, -1};
  int mid = (l + r) / 2;
  int pos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
  pair<int, int> lData = createAVLTree(l, mid - 1, keyValues, st);
  int lPos = lData.first;
  int lKey = lData.second;
  pair<int, int> rData = createAVLTree(mid + 1, r, keyValues, st);
  int rPos = rData.first;
  int rKey = rData.second;
  Node *node = new Node(keyValues[mid][0], keyValues[mid][1], 0);
  cout << "Node created and isDummy is: " << node->isDummy << endl;
  node->pos = pos;
  node->leftPos = lPos;
  node->rightPos = rPos;
  node->leftKey = lKey;
  node->rightKey = rKey;
  st->insert(node);
  return {pos, keyValues[mid][0]};
}

void DORAM::fetchPath(int bucketId, Stash *st)
{
  while (bucketId > 0)
  {
    for (int i = 0; i < Z; i++)
    {
      if (buckets[bucketId][i]->isDummy)
      {
        break;
      }
      st->insert(buckets[bucketId][i]);
      buckets[bucketId][i]->isDummy = 1;
    }
    bucketId /= 2;
  }
  cout << "State of stash after fetching path:" << endl;
  for (int i = 0; i < (int)st->nodes.size(); i++)
  {
    cout << "Key: " << st->nodes[i]->key << " isDummy flag: " << st->nodes[i]->isDummy << endl;
  }
  cout << "End of state of stash" << endl;
}

int DORAM ::access(int key, int op, int newVal)
{
  if (op == 0)
  {
    cout << "Reading key: " << key << endl;
  }
  else
  {
    cout << "Writing key: " << key << endl;
  }
  Stash *st = mainStash;
  int bucketId = rootPos;
  int eleToLookFor = rootKey;

  int randNewPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
  rootPos = randNewPos;
  cout << "Root pos changed. New root Pos: " << rootPos << endl;
  int ct = 0;
  int nextOldPos = -1, nextNewPos = -1;
  int retVal = -1;
  while (true)
  {
    if (ct >= 1)
    {
      bucketId = nextOldPos;
      randNewPos = nextNewPos;
      cout << "Fetching for: " << eleToLookFor << " at pos: " << bucketId << endl;
      cout << "Its new pos: " << randNewPos << endl;
    }
    int currPth = bucketId;
    cout << "Fetching path for bucketId: " << bucketId << endl;
    fetchPath(bucketId, st);
    cout << "Evicting stash" << endl;
    vector<int> nextData = st->evictStash(this, key, eleToLookFor, op, newVal, randNewPos, currPth);
    // oldChildPos, newChildPos, eleToLookFor
    // search eleToLookFor at oldChildPos and send it to newChildPos
    nextOldPos = nextData[0];
    nextNewPos = nextData[1];
    eleToLookFor = nextData[2];
    retVal = nextData[3];
    if (retVal != -1)
    {
      cout << "Returning value: " << retVal << endl;
      return retVal;
    }

    cout << "Next Search for: " << eleToLookFor << " at pos: " << nextOldPos << endl;
    cout << endl;
    if (nextNewPos == -1)
    {
      return retVal;
      break;
    }
    // bucketId = nextNewPos;
    ct++;
  }
}

// class DORAM
// {
// public:
//   vector<vector<Node *>> buckets;
//   int N;
//   int Z;
//   int treeHeight;
//   int numLeaves;
//   int rootPos;
//   int rootKey;

//   void initialize(vector<vector<int>> &keyValues)
//   {
//     Stash *st = new Stash(treeHeight);
//     pair<int, int> rootData = createAVLTree(0, keyValues.size() - 1, keyValues, st);
//     rootPos = rootData.first;
//     rootKey = rootData.second;
//     initialEviction(st);
//   }

//   void initialEviction(Stash *st)
//   {
//     // iterate through the stash and evict all the nodes
//     for (int i = 0; i < st->nodes.size(); i++)
//     {
//       int targetBucketId = st->nodes[i]->pos;
//       while (targetBucketId >= 0)
//       {
//         int placaed = 0;
//         for (int j = 0; j < Z; j++)
//         {
//           if (buckets[targetBucketId][j]->isDummy)
//           {
//             buckets[targetBucketId][j] = st->nodes[i];
//             placaed = 1;
//             break;
//           }
//         }
//         if (placaed == 1)
//         {
//           break;
//         }
//         targetBucketId /= 2;
//       }
//     }
//   }

//   pair<int, int> createAVLTree(int l, int r, vector<vector<int>> &keyValues, Stash *st)
//   {
//     if (l > r)
//       return {-1, -1};
//     int mid = (l + r) / 2;
//     int pos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
//     pair<int, int> lData = createAVLTree(l, mid - 1, keyValues, st);
//     int lPos = lData.first;
//     int lKey = lData.second;
//     pair<int, int> rData = createAVLTree(mid + 1, r, keyValues, st);
//     int rPos = rData.first;
//     int rKey = rData.second;
//     Node *node = new Node(keyValues[mid][0], keyValues[mid][1], 0);
//     node->pos = pos;
//     node->leftPos = lPos;
//     node->rightPos = rPos;
//     node->leftKey = lKey;
//     node->rightKey = rKey;
//     st->insert(node);
//     return {pos, keyValues[mid][0]};
//   }

//   void fetchPath(int bucketId, Stash *st)
//   {
//     while (bucketId >= 0)
//     {
//       for (int i = 0; i < Z; i++)
//       {
//         if (buckets[bucketId][i]->isDummy)
//         {
//           break;
//         }
//         st->insert(buckets[bucketId][i]);
//       }
//       bucketId /= 2;
//     }
//   }

//   void access(int key, int op, Stash *st)
//   {
//     int bucketId = rootPos;
//     while (true)
//     {
//       fetchPath(bucketId, st);
//       int nextPos = st->evictStash(key, buckets[bucketId][0]->key);
//       if (nextPos == -1)
//       {
//         break;
//       }
//       bucketId = nextPos;
//     }
//   }
// };