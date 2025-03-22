#pragma once
#include "node.h"
#include "stash.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
using namespace std;

class domap
{
public:
  // Buckets now store the encrypted (serialized & encrypted) representation of a node.
  vector<vector<string>> buckets; 

  int totalBuckets;   // total number of buckets
  int Z;              // number of nodes in a bucket
  int rootHeight;     // height of the tree
  int numLeaves;      // number of leaves in the tree
  int rootPos;        // position of the root node
  string rootKey;     // key of the root node

  // The main stash (kept in plaintext)
  stash mainStash;                                 
  // Mapping from key to a value pair (for additional indexing/lookup)
  unordered_map<string, pair<int, int>> keyValues; 

  // Constructor
  domap(vector<vector<int>> &givenKeyValues, int Z);

  // Initializes the ORAM tree structure (and encryption keys)
  void initialize();

  // Performs initial eviction after initialization
  void initialEviction();

  // Creates an AVL tree during initialization (nodes are inserted into buckets in encrypted form)
  pair<int, pair<int, string>> createAVLTree(int l, int r, vector<node> &tempNodes);

  // Fetches the path from the tree: the entire path is read and decrypted (deserialization happens only in the stash)
  void fetchPath(int bucketId);

  // Inserts a node into the tree (not shown here; you can implement similarly)
  void insertNode(int key, pair<int, int> value);

  // Prints the state of the tree (for debugging)
  void printState();

  // Access function: calls the underlying access routine
  pair<int, int> accessNode(string &key, int op, pair<int, int> newVal);

  // Access a node from the tree (will first fetch the entire path into the stash)
  node access(string &key, string &currKey, int bucketId, int newPos, int op, pair<int, int> newVal);

  // Evicts nodes from the stash back into the tree
  node evictStash(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal);

  // Adds a new edge to the graph
  void addEdge(int u, int v, int w);

  // Adds an edge directly by string key and value pair
  void addEdgeByString(string k1, pair<int, int> val);

  // Checks whether a particular key exists
  bool contains(string &key);

  // (Optional) Performs an eviction operation on the stash
  void performInitialEviction();

  // Naive eviction (for correctness verification)
  node naiveEviction(string &key, string &currKey, int newPos, int currPos, int op, pair<int, int> newVal);
};
