#pragma once
#include "node.h"
#include <vector>
using namespace std;

class domap;

class stash
{
public:
  vector<node> nodes;
  int treeHeight;
  stash();
  stash(int treeHeight);
  void insert(node node);
  vector<int> evictStash(domap tr, int keyToFind, int keyToFindInStash, int op, int newVal, int newPos, int currPth);
  int calculateLowestPossibleLevel(int currBucketId, int assignedBucketId, int treeHeight);
  int findIntersectingBucket(int a, int b);
  void printStash();
};