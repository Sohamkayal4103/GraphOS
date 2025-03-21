#include "pq.h"
#include "helper.h"

using namespace std;

void Priority_queue::insert(int key, int value)
{
  // generate random leaf position
  helper h;
  // cout << "Tree height: " << treeHeight << endl;
  int randomLeaf = h.generateRandomNumber(pow(2, treeHeight - 1), N - 1);
  // cout << "Random leaf: " << randomLeaf << endl;
  int leafPos = randomLeaf;
  int tempBucketId = randomLeaf;
  int placed = 0;
  while (tempBucketId > 0)
  {
    // cout << "tempBucketId: " << tempBucketId << endl;
    if (placed == 0)
    {
      for (int i = 0; i < Z; i++)
      {

        if (tree[tempBucketId][i].isDummy == 1)
        {
          int currSubtreeMin = tree[tempBucketId][i].subtreeMin;
          int leftChildBucket = tempBucketId * 2;
          int rightChildBucket = tempBucketId * 2 + 1;
          if (rightChildBucket >= N)
          {
            rightChildBucket = tempBucketId;
          }
          if (leftChildBucket >= N)
          {
            leftChildBucket = tempBucketId;
          }
          // cout << "Left child bucket: " << leftChildBucket << endl;
          // cout << "Right child bucket: " << rightChildBucket << endl;
          tree[tempBucketId][i] = node(key, value, leafPos, log2(tempBucketId), -1, -1, -1, -1, -1, -1, min({key, tree[leftChildBucket][0].subtreeMin, tree[rightChildBucket][0].subtreeMin, currSubtreeMin}), tree[leftChildBucket][0].subtreeMin, tree[rightChildBucket][0].subtreeMin, 0);
          placed = 1;
          // cout << "Placed at bucket: " << tempBucketId << " index: " << i << endl;
          // cout << "Subtree min: " << tree[tempBucketId][i].subtreeMin << endl;
          break;
        }
      }
      if (placed == 0)
      {
        tempBucketId /= 2;
      }
    }
    else
    {
      int tempSubtreeMin = INT_MAX;
      for (int i = 0; i < Z; i++)
      {
        if (tree[tempBucketId][i].isDummy == 0)
        {
          tempSubtreeMin = min(tempSubtreeMin, tree[tempBucketId][i].subtreeMin);
        }
      }
      // cout << "tempSubtreeMin in this bucket now: " << tempSubtreeMin << endl;
      int leftChildBucket = tempBucketId * 2;
      int rightChildBucket = tempBucketId * 2 + 1;
      if (rightChildBucket >= N)
      {
        rightChildBucket = tempBucketId;
      }
      if (leftChildBucket >= N)
      {
        leftChildBucket = tempBucketId;
      }
      // cout << "subtreeMin of left child: " << tree[leftChildBucket][0].subtreeMin << endl;
      // cout << "subtreeMin of right child: " << tree[rightChildBucket][0].subtreeMin << endl;
      tempSubtreeMin = min({tree[leftChildBucket][0].subtreeMin, tree[rightChildBucket][0].subtreeMin, tempSubtreeMin});
      // cout << "final tempSubtreeMin: " << tempSubtreeMin << endl;
      for (int i = 0; i < Z; i++)
      {
        tree[tempBucketId][i].subtreeMin = min(tempSubtreeMin, key);
      }
      tempBucketId /= 2;
    }
  }
}

pair<int, int> Priority_queue::extractMin()
{
  int overAllSubtreeMin = tree[1][0].subtreeMin;
  // cout << "overAllSubtreeMin: " << overAllSubtreeMin << endl;
  int neededValue = -1;
  int tempBucketId = 1;
  int bucketIdNeeded = -1;
  int gotIt = 0;
  while (tempBucketId < N)
  {
    int leftChildBucket = tempBucketId * 2;
    int rightChildBucket = tempBucketId * 2 + 1;

    // first check the current bucket's key and overAllSubtreeMin
    for (int i = 0; i < Z; i++)
    {
      if (tree[tempBucketId][i].isDummy == 0 && tree[tempBucketId][i].key == overAllSubtreeMin)
      {
        // found the key
        bucketIdNeeded = tempBucketId;
        gotIt = 1;
        neededValue = tree[tempBucketId][i].value;
        break;
      }
    }
    if (gotIt == 1)
    {
      break;
    }
    else
    {
      if (rightChildBucket >= N || leftChildBucket >= N)
      {
        cout << "Something went wrong. Key not found" << endl;
        break;
      }
      if (tree[leftChildBucket][0].subtreeMin < tree[rightChildBucket][0].subtreeMin)
      {
        tempBucketId = leftChildBucket;
      }
      else
      {
        tempBucketId = rightChildBucket;
      }
    }
  }

  // node replaced with dummy
  for (int i = 0; i < Z; i++)
  {
    if (tree[bucketIdNeeded][i].isDummy == 0 && tree[bucketIdNeeded][i].key == overAllSubtreeMin)
    {
      tree[bucketIdNeeded][i] = node(-1, -1, bucketIdNeeded, -1, -1, -1, -1, -1, -1, -1, INT_MAX, INT_MAX, INT_MAX, 1);
      break;
    }
  }

  // update the subtree of that bucket and all its ancestors
  tempBucketId = bucketIdNeeded;
  while (tempBucketId > 0)
  {
    int tempSubtreeMin = INT_MAX;
    for (int i = 0; i < Z; i++)
    {
      if (tree[tempBucketId][i].isDummy == 0)
      {
        tempSubtreeMin = min(tempSubtreeMin, tree[tempBucketId][i].key);
      }
    }
    int leftChildBucket = tempBucketId * 2;
    int rightChildBucket = tempBucketId * 2 + 1;
    if (rightChildBucket >= N)
    {
      rightChildBucket = tempBucketId;
    }
    if (leftChildBucket >= N)
    {
      leftChildBucket = tempBucketId;
    }
    tempSubtreeMin = min({tempSubtreeMin, tree[leftChildBucket][0].subtreeMin, tree[rightChildBucket][0].subtreeMin});
    for (int i = 0; i < Z; i++)
    {
      tree[tempBucketId][i].subtreeMin = tempSubtreeMin;
    }
    tempBucketId /= 2;
  }
  return {make_pair(overAllSubtreeMin, neededValue)};
}