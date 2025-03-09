#include <iostream>
using namespace std;

class Priority_queue
{
public:
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
  void insert(pair<int, int> &data);
  pair<int, int> extract_min();
  void print();
  int size();
  bool empty();
};