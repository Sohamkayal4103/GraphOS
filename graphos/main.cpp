#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Graph_Processing.h"
using namespace std;

int main()
{
  cout << "Hello from graphos!" << endl;
  // Take input from user
  // int n;
  // cin >> n;
  // int u, v, w;
  vector<vector<int>> data = {{1, 2, 4}, {2, 3, 5}, {1, 3, 6}};

  // for (int i = 0; i < n; i++)
  // {
  //   cin >> u >> v >> w;
  //   vector<int> temp;
  //   temp.push_back(u);
  //   temp.push_back(v);
  //   temp.push_back(w);
  //   data.push_back(temp);
  // }

  domap omixpp = domap(data, 4);
  omixpp.printState();

  GraphProcessing gp;

  gp.bfs(omixpp, 1);

  return 0;
}