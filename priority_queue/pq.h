#include "node.h"
#include <iostream>
using namespace std;
class node;

class Priority_queue
{
public:
  int N;
  int Z;
  int rootKey;
  int rootPos;
  int treeHeight;
  vector<vector<node>> tree;
  vector<int> bucketMin;

  Priority_queue(int n, int z)
  {
    N = n;
    Z = z;
    tree.resize(N);
    bucketMin.resize(N);
    for (int i = 0; i < N; i++)
    {
      tree[i].resize(Z, node(-1, -1, i, -1, -1, -1, -1, -1, -1, -1, INT_MAX, INT_MAX, INT_MAX, 1));
    }
    treeHeight = ceil(log2(N));
    for (int i = 0; i < N; i++)
    {
      // initialize all nodes as dummy key = -1 , value = -1, pos = that bucket, height = -1, leftPos = -1, leftKey = -1, letHeight = -1, rightPos = -1, rightKey = -1, rightHeight = -1, subtreeMin = INT_MAX, leftTreeMin = INT_MAX, rightTreeMin = INT_MAX, isDummy = 1
      for (int j = 0; j < Z; j++)
      {
        tree[i][j] = node(-1, -1, i, -1, -1, -1, -1, -1, -1, -1, INT_MAX, INT_MAX, INT_MAX, 1);
      }
    }
  }

  void insert(int key, int value);
  pair<int, int> extractMin();
};