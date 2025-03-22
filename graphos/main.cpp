#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include "Graph_Processing.h"
#include "domap.h"

using namespace std;
using namespace std::chrono;

// Dummy implementation for file loading (replace with actual file I/O if needed)
vector<vector<int>> loadGraphFromFile(const string &filename)
{
  vector<vector<int>> data;
  ifstream file(filename);
  string line;

  if (!file.is_open())
  {
    cerr << "Error opening file: " << filename << endl;
    return data;
  }

  while (getline(file, line))
  {
    stringstream ss(line);
    string value;
    vector<int> edge;

    while (getline(ss, value, ','))
    {
      edge.push_back(stoi(value));
    }

    if (edge.size() == 3)
    {
      data.push_back(edge);
    }
  }

  file.close();
  return data;
}

int main()
{
  cout << "Welcome to graphos!" << endl;
  cout << "Select mode:" << endl;
  cout << "1. Single Test" << endl;
  cout << "2. Benchmark Simulation" << endl;

  int mode;
  cin >> mode;

  if (mode == 1)
  {
    vector<vector<int>> data = {{1, 2, 1}, {2, 3, 5}, {1, 4, 3}, {4, 5, 1}};
    cout << "Select algorithm:" << endl;
    cout << "1. BFS" << endl;
    cout << "2. DFS" << endl;
    cout << "3. Dijkstra" << endl;
    int algo;
    cin >> algo;

    auto initStart = high_resolution_clock::now();
    domap omixpp(data, 4);
    auto initEnd = high_resolution_clock::now();
    auto initTime = duration_cast<microseconds>(initEnd - initStart);
    cout << "Initialization time: " << initTime.count() << " microseconds" << endl;

    omixpp.printState();
    GraphProcessing gp;
    gp.setDebugMode(true);
    if (algo == 1)
    {
      auto start = high_resolution_clock::now();
      gp.bfs(omixpp, 1);
      auto end = high_resolution_clock::now();
      cout << "BFS time: " << duration_cast<microseconds>(end - start).count() << " microseconds" << endl;
    }
    else if (algo == 2)
    {
      auto start = high_resolution_clock::now();
      gp.dfs(omixpp, 1);
      auto end = high_resolution_clock::now();
      cout << "DFS time: " << duration_cast<microseconds>(end - start).count() << " microseconds" << endl;
    }
    else if (algo == 3)
    {
      auto start = high_resolution_clock::now();
      gp.dijkstra(5, 4, omixpp, 1);
      auto end = high_resolution_clock::now();
      cout << "Dijkstra time: " << duration_cast<microseconds>(end - start).count() << " microseconds" << endl;
    }
    else
    {
      cout << "Invalid algorithm selection." << endl;
    }
  }
  else if (mode == 2)
  {
    vector<string> graphFiles = {
        "../graphGenerator/graph_100",
        "../graphGenerator/graph_200",
        "../graphGenerator/graph_300",
        "../graphGenerator/graph_400",
        "../graphGenerator/graph_500"};

    for (const auto &filename : graphFiles)
    {
      cout << "\nProcessing " << filename << "..." << endl;
      vector<vector<int>> data = loadGraphFromFile(filename);
      if (data.empty())
      {
        cout << "No data loaded from " << filename << endl;
        continue;
      }

      int numNodes = 0;
      for (const auto &edge : data)
        numNodes = max(numNodes, max(edge[0], edge[1]));
      numNodes++; // account for indexing

      auto initStart = high_resolution_clock::now();
      domap omixpp(data, numNodes);
      auto initEnd = high_resolution_clock::now();
      cout << "Initialization time: " << duration_cast<microseconds>(initEnd - initStart).count() << " microseconds" << endl;

      GraphProcessing gp;
      gp.setDebugMode(false);
      auto bfsStart = high_resolution_clock::now();
      gp.bfs(omixpp, 1);
      auto bfsEnd = high_resolution_clock::now();
      cout << "BFS time: " << duration_cast<microseconds>(bfsEnd - bfsStart).count() << " microseconds" << endl;

      auto dfsStart = high_resolution_clock::now();
      gp.dfs(omixpp, 1);
      auto dfsEnd = high_resolution_clock::now();
      cout << "DFS time: " << duration_cast<microseconds>(dfsEnd - dfsStart).count() << " microseconds" << endl;

      auto dijkstraStart = high_resolution_clock::now();
      gp.dijkstra(numNodes, numNodes / 2, omixpp, 1);
      auto dijkstraEnd = high_resolution_clock::now();
      cout << "Dijkstra time: " << duration_cast<microseconds>(dijkstraEnd - dijkstraStart).count() << " microseconds" << endl;
    }
  }
  else
  {
    cout << "Invalid mode selected." << endl;
    return 1;
  }

  return 0;
}
