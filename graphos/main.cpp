#include "../path__doram/doram.h"
#include "../path__doram/stash.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "DOMAP.h"
#include "Graph_Processing.h"
using namespace std;

int main()
{
  cout << "Hello from graphos!" << endl;
  // Take input from user
  // int n;
  // cin >> n;
  // int u, v, w;
  vector<vector<int>> data = {{1, 2, 4}, {2, 4, 1}, {1, 3, 2}, {3, 4, 5}};

  // for (int i = 0; i < n; i++)
  // {
  //   cin >> u >> v >> w;
  //   vector<int> temp;
  //   temp.push_back(u);
  //   temp.push_back(v);
  //   temp.push_back(w);
  //   data.push_back(temp);
  // }

  // Initialize the graph
  DOMAP *domap = new DOMAP(data);
  domap->print();

  // Initialize the GraphProcessing object
  GraphProcessing *graphProcessing = new GraphProcessing();

  // Perform BFS
  // cout << "BFS:" << endl;
  // cout << "---------------------" << endl;
  // graphProcessing->bfs(domap, 1);
  // domap->print();

  // cout << "---------------------" << endl;

  // Perform DFS
  // cout << "DFS:" << endl;
  // cout << "---------------------" << endl;
  // graphProcessing->dfs(domap, 1);
  // domap->print();
  // domap->printStk();

  // perform Dijkstra
  cout << "Dijkstra:" << endl;
  cout << "---------------------" << endl;
  graphProcessing->dijkstra(5, 5, domap, 1);
  domap->print();
  domap->printDist();

  return 0;
}