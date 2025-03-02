#include "doram.h"
#include "stash.h"
#include "helper.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

class DORAM;

Stash::Stash(int treeHeight)
{
  this->treeHeight = treeHeight;
}

void Stash::insert(Node *node)
{
  // cout << "Inserting key: " << node->key << " in the stash and isDummy: " << node->isDummy << endl;
  Node *newNode = new Node(node->key, node->value, node->isDummy);
  newNode->pos = node->pos;
  newNode->leftPos = node->leftPos;
  newNode->rightPos = node->rightPos;
  newNode->leftKey = node->leftKey;
  newNode->rightKey = node->rightKey;
  newNode->level = node->level;
  newNode->bucketId = node->bucketId;
  nodes.push_back(newNode);
}

vector<int> Stash::evictStash(DORAM *tr, int keyToFind, int keyToFindInStash, int op, int newVal, int newPos, int currPth)
{
  // cout << "newPos by graphos eviction: " << newPos << endl;
  int fans = -1;
  cout << endl;
  cout << "Searching for key: " << keyToFindInStash << " in the stash" << endl;
  int currPath = nodes[0]->pos;
  int newChildPos = -1, oldChildPos = -1;
  int eleToLookForNext = -1;
  for (int i = 0; i < (int)nodes.size(); i++)
  {
    if (nodes[i]->key == keyToFindInStash)
    {
      cout << "Found the key: " << keyToFindInStash << " in the stash" << endl;
      cout << "it's position is: " << nodes[i]->pos << endl;
      cout << "it's left child position is: " << nodes[i]->leftPos << endl;
      cout << "it's right child position is: " << nodes[i]->rightPos << endl;
      cout << "it's level is: " << nodes[i]->level << endl;
      cout << endl;

      int pos = nodes[i]->pos;
      int leftPos = nodes[i]->leftPos;
      int rightPos = nodes[i]->rightPos;
      int leftKey = nodes[i]->leftKey;
      int rightKey = nodes[i]->rightKey;
      int sig = 0;
      if (nodes[i]->key == keyToFind)
      {
        // found the actual node we were looking for
        // return this to the user
        // op == 2 => write operation => update the value
        // op == 1 => read operation => return the value
        cout << "Found the key: " << keyToFind << " at position: " << pos << endl;
        if (op == 1)
        {
          cout << "Returning the value of key: " << nodes[i]->key << " " << nodes[i]->value << endl;
          fans = nodes[i]->value;
        }
        else
        {
          cout << "Updating the value of key: " << nodes[i]->key << " to: " << newVal << endl;
          nodes[i]->value = newVal;
          fans = -2;
        }
      }
      else if (nodes[i]->key < keyToFind)
      {
        if (rightPos != -1)
        {
          eleToLookForNext = rightKey;
          cout << "Decided to go right and look for: " << eleToLookForNext << endl;
          cout << endl;
          sig = 1;

          oldChildPos = nodes[i]->rightPos;
          newChildPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
          cout << "NewPos for the right child: " << newChildPos << endl;
          cout << "Comp b/w newPos (already calculated for current parent node) and newChildPos(calculated just new for child): " << newPos << " " << newChildPos << endl;
          cout << endl;
        }
      }
      else
      {
        if (leftPos != -1)
        {
          eleToLookForNext = leftKey;
          cout << "Decided to go left and look for: " << eleToLookForNext << endl;
          cout << endl;
          sig = 2;

          oldChildPos = nodes[i]->leftPos;
          newChildPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
          cout << "NewPos for the left child: " << newChildPos << endl;
          cout << "Comp b/w newPos (already calculated for current parent node) and newChildPos(calculated just new for child): " << newPos << " " << newChildPos << endl;
          cout << endl;
        }
      }
      cout << "Calculating new position for the key: " << nodes[i]->key << endl;
      int oldPos = nodes[i]->pos;
      cout << "treeHeight is: " << treeHeight << endl;
      cout << "Calculating a random leaf b/w: " << pow(2, treeHeight - 1) << " and " << pow(2, treeHeight) - 1 << endl;
      if (newPos == -1)
      {
        newPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
      }
      nodes[i]->pos = newPos;
      cout << "Calculating new position for the left child of the key: " << nodes[i]->key << endl;
      // generating new Positions for child
      // oldChildPos = nodes[i]->leftPos;
      // newChildPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);

      if (sig == 0)
      {
        nodes[i]->pos = newPos;
      }
      else if (sig == 1)
      {
        nodes[i]->pos = newPos;
        nodes[i]->rightPos = newChildPos;
      }
      else
      {
        nodes[i]->pos = newPos;
        nodes[i]->leftPos = newChildPos;
      }
      cout << "Key: " << nodes[i]->key << " Old Pos: " << oldPos << " New Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
      // assigning lowest possible levels to the nodes
      nodes[i]->level = calculateLowestPossibleLevel(oldPos, nodes[i]->pos, 2);
      cout << "New level of the key: " << nodes[i]->key << " is: " << nodes[i]->level << endl;
    }
    else
    {
      // assigning lowest possible levels to the nodes
      nodes[i]->level = calculateLowestPossibleLevel(currPth, nodes[i]->pos, 2);
      cout << "New level of the key: " << nodes[i]->key << " is: " << nodes[i]->level << endl;
    }
  }
  sort(nodes.begin(), nodes.end(), [](Node *a, Node *b)
       { return a->level > b->level; });
  cout << "After 1st sort: (Based on assigned levels)" << endl;
  for (int i = 0; i < (int)nodes.size(); i++)
  {
    cout << "Key: " << nodes[i]->key << " Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
  }
  cout << endl;

  int bucketId = currPath;
  int currLevel = 2;
  int p1 = 0;
  int Z = 4;

  // assigning bucketId
  while (bucketId > 0 && p1 < (int)nodes.size())
  {
    cout << "Checking for: " << bucketId << endl;
    cout << "Current level: " << currLevel << endl;
    cout << endl;
    cout << "Current Node level: " << nodes[p1]->level << endl;
    cout << endl;
    while (p1 < (int)nodes.size() && nodes[p1]->level >= currLevel)
    {
      int totalAvailable = 0;
      for (int i = 0; i < Z; i++)
      {
        if (tr->buckets[bucketId][i]->isDummy)
        {
          totalAvailable += Z - i;
          break;
        }
      }
      while (totalAvailable > 0 && p1 < (int)nodes.size() && nodes[p1]->level >= currLevel)
      {
        cout << "Deciding for key: " << nodes[p1]->key << " at bucketId: " << bucketId << " at level: " << currLevel << endl;
        cout << endl;
        nodes[p1]->level = currLevel;
        nodes[p1]->bucketId = bucketId;
        // cout << "Placed key: " << nodes[p1]->key << " at bucketId: " << bucketId << " at level: " << currLevel << endl;
        cout << endl;
        totalAvailable--;
        p1++;
      }
    }
    bucketId /= 2;
    currLevel--;
  }

  sort(nodes.begin(), nodes.end(), [](Node *a, Node *b)
       { return a->bucketId > b->bucketId; });

  cout << "After second sort: (Based on assigned buckets)" << endl;
  for (int i = 0; i < (int)nodes.size(); i++)
  {
    cout << "Key: " << nodes[i]->key << " Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
  }
  cout << endl;

  // placement Starts here
  bucketId = currPath;
  currLevel = 2;
  p1 = 0;
  Z = 4;
  p1 = 0;
  cout << "Placement begins: " << endl;
  while (bucketId > 0 && p1 < (int)nodes.size())
  {
    cout << "Checking for: " << bucketId << endl;
    cout << "Current level: " << currLevel << endl;
    cout << endl;
    while (p1 < (int)nodes.size() && nodes[p1]->bucketId == bucketId)
    {
      cout << "reached inside" << endl;
      for (int i = 0; i < Z; i++)
      {
        if (tr->buckets[bucketId][i]->isDummy)
        {
          tr->buckets[bucketId][i] = nodes[p1];
          cout << "isDummy flag for nodes[p1]: " << nodes[p1]->isDummy << endl;
          cout << "Placed key: " << nodes[p1]->key << " at bucketId: " << bucketId << " at level: " << currLevel << endl;
          cout << endl;
          break;
        }
      }
      p1++;
    }
    bucketId /= 2;
    currLevel--;
  }
  cout << "After placement" << endl;
  for (int i = 0; i < (int)tr->buckets.size(); i++)
  {
    cout << "Bucket: " << i << endl;
    for (int j = 0; j < (int)tr->buckets[i].size(); j++)
    {
      if (tr->buckets[i][j]->isDummy)
      {
        cout << "Bucket: " << i << " is empty" << endl;
        break;
      }
      else
      {
        cout << "Key: " << tr->buckets[i][j]->key << " Pos: " << tr->buckets[i][j]->pos << " Level: " << tr->buckets[i][j]->level << endl;
      }
      // cout << "Key: " << tr->buckets[i][j]->key << " Pos: " << tr->buckets[i][j]->pos << " Level: " << tr->buckets[i][j]->level << endl;
    }
    cout << endl;
  }
  cout << endl;
  nodes.clear();
  nodes.resize(0);
  cout << "Eviction Completed..." << endl;
  cout << endl;
  vector<int> res = {oldChildPos, newChildPos, eleToLookForNext, fans};
  return res;
}
// while (totalAvailable > 0 && p1 < nodes.size())
// {
//   cout << "Placing key: " << nodes[p1]->key << " at bucketId: " << bucketId << " at level: " << currLevel << endl;
//   cout << endl;
//   tr->buckets[bucketId][Z - totalAvailable] = nodes[p1];
//   totalAvailable--;
//   p1++;
// }
//     }
//     bucketId /= 2;
//     currLevel--;
//   }
//   nodes.clear();
//   cout << "Eviction Completed..." << endl;
//   cout << endl;
//   return newChildPos;
// }

int Stash::calculateLowestPossibleLevel(int currBucketId, int assignedBucketId, int treeHeight)
{
  if (treeHeight == 0)
  {
    cout << "Followed this" << endl;
    return 0;
  }
  int intersectingBucket = findIntersectingBucket(currBucketId, assignedBucketId);
  int level = log2(intersectingBucket);
  return level;
}

int Stash::findIntersectingBucket(int a, int b)
{
  cout << "Finding the intersecting bucket between: " << a << " and " << b << endl;
  cout << endl;
  while (a != b)
  {
    if (a > b)
    {
      a /= 2;
    }
    else
    {
      b /= 2;
    }
  }
  cout << "Intersecting bucket is: " << a << endl;
  cout << endl;
  return a;
}

// class Stash
// {
// public:
//   vector<Node *> nodes;
//   int treeHeight;

//   Stash(int treeHeight)
//   {
//     this->treeHeight = treeHeight;
//   }

//   void insert(Node *node)
//   {
//     nodes.push_back(node);
//   }

//   int evictStash(DORAM *tr, int keyToFind, int keyToFindInStash)
//   {
//     cout << "Current state of the stash" << endl;
//     for (int i = 0; i < nodes.size(); i++)
//     {
//       cout << "Key: " << nodes[i]->key << " Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
//     }
//     cout << endl;

//     int currPath = nodes[0]->pos;
//     int newChildPos = -1;
//     for (int i = 0; i < nodes.size(); i++)
//     {
//       if (nodes[i]->key == keyToFindInStash)
//       {

//         int pos = nodes[i]->pos;
//         int leftPos = nodes[i]->leftPos;
//         int rightPos = nodes[i]->rightPos;
//         int sig = 0;
//         if (nodes[i]->key == keyToFind)
//         {
//           // found the actual node we were looking for
//           // return this to the user
//           // op == 2 => write operation => update the value
//           // op == 1 => read operation => return the value
//           cout << "Found the key: " << keyToFind << " at position: " << pos << endl;
//         }
//         else if (nodes[i]->key < keyToFind)
//         {
//           if (rightPos != -1)
//           {
//             sig = 1;
//           }
//         }
//         else
//         {
//           if (leftPos != -1)
//           {
//             sig = 2;
//           }
//         }
//         int oldPos = nodes[i]->pos;
//         int newPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);
//         newChildPos = generateRandomNumber(pow(2, treeHeight - 1), pow(2, treeHeight) - 1);

//         if (sig == 0)
//         {
//           nodes[i]->pos = newPos;
//         }
//         else if (sig == 1)
//         {
//           nodes[i]->pos = newPos;
//           nodes[i]->rightPos = newChildPos;
//         }
//         else
//         {
//           nodes[i]->pos = newPos;
//           nodes[i]->leftPos = newChildPos;
//         }
//         cout << "Key: " << nodes[i]->key << " Old Pos: " << oldPos << " New Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
//         nodes[i]->level = calculateLowestPossibleLevel(oldPos, nodes[i]->pos, 2);
//       }
//       else
//       {
//         nodes[i]->level = calculateLowestPossibleLevel(nodes[i]->pos, nodes[i]->pos, 2);
//       }
//     }
//     sort(nodes.begin(), nodes.end(), [](Node *a, Node *b)
//          { return a->level > b->level; });
//     cout << "After sorting the stash" << endl;
//     for (int i = 0; i < nodes.size(); i++)
//     {
//       cout << "Key: " << nodes[i]->key << " Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
//     }
//     cout << endl;

//     int bucketId = currPath;
//     int currLevel = 2;
//     int p1 = 0;
//     int Z = 4;

//     while (bucketId >= 0 && p1 < nodes.size())
//     {
//       while (nodes[p1]->level <= currLevel)
//       {
//         int totalAvailable = 0;
//         for (int i = 0; i < Z; i++)
//         {
//           if (tr->buckets[bucketId][i]->isDummy)
//           {
//             totalAvailable += Z - i;
//             break;
//           }
//         }
//         while (totalAvailable > 0 && p1 < nodes.size())
//         {
//           nodes[p1]->pos = bucketId;
//           nodes[p1]->level = currLevel;
//           nodes[p1]->bucketId = bucketId;
//           totalAvailable--;
//           p1++;
//         }
//       }
//       bucketId /= 2;
//       currLevel--;
//     }

//     sort(nodes.begin(), nodes.end(), [](Node *a, Node *b)
//          { return a->bucketId > b->bucketId; });

//     cout << "After second sort: " << endl;
//     for (int i = 0; i < nodes.size(); i++)
//     {
//       cout << "Key: " << nodes[i]->key << " Pos: " << nodes[i]->pos << " Level: " << nodes[i]->level << endl;
//     }
//     cout << endl;

//     bucketId = currPath;
//     currLevel = 2;
//     p1 = 0;
//     Z = 4;
//     while (bucketId >= 0 && p1 < nodes.size())
//     {
//       while (nodes[p1]->level <= currLevel)
//       {
//         int totalAvailable = 0;
//         for (int i = 0; i < Z; i++)
//         {
//           if (tr->buckets[bucketId][i]->isDummy)
//           {
//             totalAvailable += Z - i;
//             break;
//           }
//         }
//         while (totalAvailable > 0 && p1 < nodes.size())
//         {
//           tr->buckets[bucketId][Z - totalAvailable] = nodes[p1];
//           totalAvailable--;
//           p1++;
//         }
//       }
//       bucketId /= 2;
//       currLevel--;
//     }
//     nodes.clear();
//     cout << "Eviction Completed" << endl;
//     return newChildPos;
//   }

//   int calculateLowestPossibleLevel(int currBucketId, int assignedBucketId, int treeHeight)
//   {
//     int intersectingBucket = findIntersectingBucket(currBucketId, assignedBucketId);
//     int level = log2(intersectingBucket);
//     return level;
//   }

//   int findIntersectingBucket(int a, int b)
//   {
//     while (a != b)
//     {
//       if (a > b)
//       {
//         a /= 2;
//       }
//       else
//       {
//         b /= 2;
//       }
//     }
//     return a;
//   }
// };