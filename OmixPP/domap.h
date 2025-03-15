#pragma once
#include "node.h"
#include "stash.h"
#include <vector>
using namespace std;

class domap
{
public:
  vector<vector<node>> buckets; // vector of buckets where each bucket is a vector of nodes
  int totalBuckets;             // total number of buckets
  int Z;                        // number of nodes in a bucket
  int rootHeight;               // height of the tree
  int numLeaves;                // number of leaves in the tree
  int rootPos;                  // position of the root node
  string rootKey;               // key of the root node

  stash mainStash;                                 // defining main stash
  unordered_map<string, pair<int, int>> keyValues; // mapping key to values

  // Constructor
  domap(vector<vector<int>> &givenKeyValues, int Z);

  // initilaizes the omixpp
  void initialize();

  // function for intial eviction after initialization
  void initialEviction();

  // function to create AVL tree while initializing
  pair<int, pair<int, string>> createAVLTree(int l, int r, vector<node> &tempNodes);

  // fetches the path and puts all nodes in stash
  void fetchPath(int bucketId);

  // inserts a node in the tree
  void insertNode(int key, pair<int, int> value);

  // to print the state of the oram tree
  void printState();

  // to call the access function and dependent calls
  pair<int, int> accessNode(string &key, int op, pair<int, int> newVal);

  // to access the node
  node access(string &key, string &currKey, int bucketId, int newPos, int op, pair<int, int> newVal);

  // to evict the stash
  node evictStash(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal);

  // add new edge to the graph
  void addEdge(int u, int v, int w);

  // to add edge directly by key in string and value
  void addEdgeByString(string k1, pair<int, int> val);

  // to check whether a particular key exists
  bool contains(string &key);

  // to perform initial eviction
  void performInitialEviction();

  // naive eviction for correctness
  node naiveEviction(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal);
};