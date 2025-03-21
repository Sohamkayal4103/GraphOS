#include "pq.h"
using namespace std;

int main()
{
  cout << "testing oblivious priority queue" << endl;
  Priority_queue pq(8, 1);

  pq.insert(3, 2);
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  pq.insert(5, 1);
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  pq.insert(1, 4);
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  int leftChildBucket = 1 * 2;
  int rightChildBucket = 1 * 2 + 1;
  cout << "subtree min of left subtree: " << pq.tree[leftChildBucket][0].subtreeMin << endl;
  cout << "subtree min of right subtree: " << pq.tree[rightChildBucket][0].subtreeMin << endl;

  cout << "----------------------------" << endl;

  pair<int, int> minPair = pq.extractMin();
  cout << "minPair.first: " << minPair.first << endl;
  cout << "minPair.second: " << minPair.second << endl;
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  cout << "----------------------------" << endl;

  leftChildBucket = 1 * 2;
  rightChildBucket = 1 * 2 + 1;

  cout << "subtree min of left subtree: " << pq.tree[leftChildBucket][0].subtreeMin << endl;
  cout << "subtree min of right subtree: " << pq.tree[rightChildBucket][0].subtreeMin << endl;

  cout << "----------------------------" << endl;
  minPair = pq.extractMin();
  cout << "minPair.first: " << minPair.first << endl;
  cout << "minPair.second: " << minPair.second << endl;
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  cout << "----------------------------" << endl;
  leftChildBucket = 1 * 2;
  rightChildBucket = 1 * 2 + 1;
  cout << "subtree min of left subtree: " << pq.tree[leftChildBucket][0].subtreeMin << endl;
  cout << "subtree min of right subtree: " << pq.tree[rightChildBucket][0].subtreeMin << endl;
  cout << "----------------------------" << endl;

  cout << "Inserting 10, 3" << endl;
  pq.insert(10, 3);
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  cout << "----------------------------" << endl;
  leftChildBucket = 1 * 2;
  rightChildBucket = 1 * 2 + 1;
  cout << "subtree min of left subtree: " << pq.tree[leftChildBucket][0].subtreeMin << endl;
  cout << "subtree min of right subtree: " << pq.tree[rightChildBucket][0].subtreeMin << endl;
  cout << "----------------------------" << endl;

  minPair = pq.extractMin();
  cout << "minPair.first: " << minPair.first << endl;
  cout << "minPair.second: " << minPair.second << endl;
  cout << "Subtree min: " << pq.tree[1][0].subtreeMin << endl;
  cout << "----------------------------" << endl;
  leftChildBucket = 1 * 2;
  rightChildBucket = 1 * 2 + 1;
  cout << "subtree min of left subtree: " << pq.tree[leftChildBucket][0].subtreeMin << endl;
  cout << "subtree min of right subtree: " << pq.tree[rightChildBucket][0].subtreeMin << endl;
  cout << "----------------------------" << endl;
  return 0;
}