#ifndef STASH_H
#define STASH_H

#include "node.cpp"
#include <vector>
#include <algorithm> // For std::sort
#pragma once

using namespace std;

class DORAM;

class Stash
{
public:
    vector<Node *> nodes;
    int treeHeight;
    Stash(int treeHeight);
    // Function declarations
    void insert(Node *node);
    vector<int> evictStash(DORAM *tr, int keyToFind, int keyToFindInStash, int op, int newVal, int newPos,int currPth);
    int calculateLowestPossibleLevel(int currBucketId, int assignedBucketId, int treeHeight);
    int findIntersectingBucket(int a, int b);

    // private:
    //     int generateNewPos();                          // Assuming this function exists
    //     bool isSamePath(int bucketId1, int bucketId2); // Assuming this function exists
};

#endif // STASH_H
