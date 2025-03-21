// #include <iostream>
// #include <vector>
// #include <string>
// #include <unordered_map>
// #include <algorithm>
// #include <chrono>
// #include "Graph_Processing.h"
// using namespace std;

// int main()
// {
//   cout << "Hello from graphos!" << endl;
//   // Take input from user
//   // int n;
//   // cin >> n;
//   // int u, v, w;
//   vector<vector<int>> data = {{1, 2, 1}, {2, 3, 5}, {1, 3, 3}};

//   // for (int i = 0; i < n; i++)
//   // {
//   //   cin >> u >> v >> w;
//   //   vector<int> temp;
//   //   temp.push_back(u);
//   //   temp.push_back(v);
//   //   temp.push_back(w);
//   //   data.push_back(temp);
//   // }

//   auto initTime = chrono::high_resolution_clock::now();
//   domap omixpp = domap(data, 4);
//   auto initEndTime = chrono::high_resolution_clock::now();
//   auto initDuration = chrono::duration_cast<chrono::microseconds>(initEndTime - initTime);
//   cout << "Initialization time: " << initDuration.count() << " microseconds" << endl;

//   omixpp.printState();

//   GraphProcessing gp;

//   // auto start = chrono::high_resolution_clock::now();
//   // gp.bfs(omixpp, 1);
//   // auto end = chrono::high_resolution_clock::now();
//   // auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
//   // cout << "Time taken for BFS: " << duration.count() << " microseconds" << endl;

//   // auto start = chrono::high_resolution_clock::now();
//   // gp.dfs(omixpp, 1);
//   // auto end = chrono::high_resolution_clock::now();
//   // auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
//   // cout << "Time taken for DFS: " << duration.count() << " microseconds" << endl;

//   auto start = chrono::high_resolution_clock::now();
//   gp.dijkstra(4, 3, omixpp, 1);
//   auto end = chrono::high_resolution_clock::now();
//   auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
//   cout << "Time taken for Dijkstra: " << duration.count() << " microseconds" << endl;

//   return 0;
// }

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include "Graph_Processing.h"
using namespace std;

// Function to load graph data from file
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
  cout << "Graph processing benchmark" << endl;

  // Define graph files to process
  vector<string> graphFiles = {
      "../graphGenerator/graph_100",
      "../graphGenerator/graph_200",
      "../graphGenerator/graph_300",
      "../graphGenerator/graph_400",
      "../graphGenerator/graph_500"};

  // Create results file
  ofstream resultsFile("benchmark_results.csv");
  if (!resultsFile.is_open())
  {
    cerr << "Failed to open results file" << endl;
    return 1;
  }

  // Write CSV header
  resultsFile << "Graph,Nodes,Edges,Init_Time_us,BFS_Time_us,DFS_Time_us,Dijkstra_Time_us" << endl;

  GraphProcessing gp;

  // Process each graph file
  for (const string &filename : graphFiles)
  {
    cout << "Processing " << filename << "..." << endl;

    // Load graph data
    vector<vector<int>> data = loadGraphFromFile(filename);
    if (data.empty())
    {
      cerr << "Error: Empty graph data for " << filename << endl;
      continue;
    }

    // Find number of nodes (assuming node IDs start from 0 or 1)
    int numNodes = 0;
    for (const auto &edge : data)
    {
      numNodes = max(numNodes, max(edge[0], edge[1]));
    }
    numNodes++; // Account for 0-indexing

    // Initialize graph
    auto initTime = chrono::high_resolution_clock::now();
    domap omixpp = domap(data, numNodes);
    auto initEndTime = chrono::high_resolution_clock::now();
    auto initDuration = chrono::duration_cast<chrono::microseconds>(initEndTime - initTime);
    cout << "  Initialization time: " << initDuration.count() << " microseconds" << endl;

    // Run BFS and measure time
    auto bfsStart = chrono::high_resolution_clock::now();
    gp.bfs(omixpp, 1);
    auto bfsEnd = chrono::high_resolution_clock::now();
    auto bfsDuration = chrono::duration_cast<chrono::microseconds>(bfsEnd - bfsStart);
    cout << "  BFS time: " << bfsDuration.count() << " microseconds" << endl;

    // Run DFS and measure time
    auto dfsStart = chrono::high_resolution_clock::now();
    gp.dfs(omixpp, 1);
    auto dfsEnd = chrono::high_resolution_clock::now();
    auto dfsDuration = chrono::duration_cast<chrono::microseconds>(dfsEnd - dfsStart);
    cout << "  DFS time: " << dfsDuration.count() << " microseconds" << endl;

    // Run Dijkstra and measure time
    auto dijkstraStart = chrono::high_resolution_clock::now();
    gp.dijkstra(numNodes, numNodes / 2, omixpp, 1); // Start at node 1, end at middle node
    auto dijkstraEnd = chrono::high_resolution_clock::now();
    auto dijkstraDuration = chrono::duration_cast<chrono::microseconds>(dijkstraEnd - dijkstraStart);
    cout << "  Dijkstra time: " << dijkstraDuration.count() << " microseconds" << endl;

    // Write results to file
    resultsFile << filename << ","
                << numNodes << ","
                << data.size() << ","
                << initDuration.count() << ","
                << bfsDuration.count() << ","
                << dfsDuration.count() << ","
                << dijkstraDuration.count() << endl;
  }

  resultsFile.close();
  cout << "Benchmark complete. Results written to benchmark_results.csv" << endl;

  return 0;
}