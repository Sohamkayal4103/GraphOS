#ifndef DORAM_H
#define DORAM_H

// #include "node.cpp"
#include "stash.h"
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

class DORAM
{
public:
    vector<vector<Node *>> buckets;
    int N;
    int Z;
    int treeHeight;
    int numLeaves;
    int rootPos;
    int rootKey;
    Stash *mainStash;

    // Constructor
    DORAM(int maxNodes);

    // Function declarations
    void initialize(vector<vector<int>> &keyValues);
    void initialEviction(Stash *st);
    pair<int, int> createAVLTree(int l, int r, vector<vector<int>> &keyValues, Stash *st);
    void fetchPath(int bucketId, Stash *st);
    int access(int key, int op, int newVal);
};

#endif // DORAM_H
