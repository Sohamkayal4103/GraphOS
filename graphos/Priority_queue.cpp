#include "Priority_queue.h"
#include <iostream>
using namespace std;

void Priority_queue::insert(pair<int, int> &data)
{
  pq.push(data);
}

pair<int, int> Priority_queue::extract_min()
{
  cout << "Extracting min" << endl;
  if (pq.empty())
  {
    cout << "Priority queue is empty" << endl;
    return {-1, -1};
  }
  pair<int, int> top = pq.top();
  cout << "Top: " << top.first << " " << top.second << endl;
  cout << "----------------" << endl;
  pq.pop();
  return top;
}

void Priority_queue::print()
{
  vector<pair<int, int>> v;
  while (!pq.empty())
  {
    pair<int, int> top = pq.top();
    cout << top.first << " " << top.second << endl;
    pq.pop();
  }
  for (int i = 0; i < v.size(); i++)
  {
    pq.push(v[i]);
  }
}

int Priority_queue::size()
{
  return pq.size();
}

bool Priority_queue::empty()
{
  return pq.empty();
}