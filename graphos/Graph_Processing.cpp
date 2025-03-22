#include "Graph_Processing.h"
#include "Priority_queue.h"
#include <iostream>
#include <climits>
using namespace std;

// Setter function for debug mode.
void GraphProcessing::setDebugMode(bool modeFinal)
{
  this->debugMode = modeFinal;
}

void GraphProcessing::bfs(domap graph, int start)
{
  if (debugMode)
    cout << "BFS Traversal:" << endl;

  int source = start;
  int qCnt = 0, curQcnt = 0;
  bool outerL = true;
  string key = "InQ" + to_string(qCnt);
  graph.addEdgeByString(key, {start, 0});

  // Mark the start node as visited.
  string visKey = "visited" + to_string(start);
  if (!graph.contains(visKey))
    graph.addEdgeByString(visKey, {1, 1});
  else
    graph.accessNode(visKey, 1, {1, 1});
  qCnt++;

  int cnt = 1;
  while (qCnt != curQcnt)
  {
    string qKey = "InQ" + to_string(curQcnt);
    int curNode = graph.accessNode(qKey, 0, {0, 0}).first;
    if (outerL)
    {
      if (debugMode)
        cout << "Visiting node: " << curNode << endl;
      source = curNode;
      curQcnt++;
      cnt = 1;
    }
    string nextKey = "Eout" + to_string(source) + "_" + to_string(cnt);
    if (graph.contains(nextKey))
    {
      int nextNode = graph.accessNode(nextKey, 0, {0, 0}).first;
      // Mark the neighbor as visited.
      string nextVisKey = "visited" + to_string(nextNode);
      if (!graph.contains(nextVisKey))
        graph.addEdgeByString(nextVisKey, {1, 1});
      else
        graph.accessNode(nextVisKey, 1, {1, 1});
      // Enqueue the neighbor.
      string nextQKey = "InQ" + to_string(qCnt);
      graph.addEdgeByString(nextQKey, {nextNode, 0});
      if (debugMode)
        cout << "Discovered edge from " << source << " to " << nextNode << endl;
      qCnt++;
      outerL = false;
      cnt++;
    }
    else
    {
      outerL = true;
    }
  }
}

void GraphProcessing::dijkstra(int V, int E, domap graph, int start)
{
  // Initialize distances for all vertices.
  for (int i = 1; i <= V; i++)
  {
    string key = "Dist" + to_string(i);
    graph.addEdgeByString(key, {INT_MAX, 0});
  }
  string startKey = "Dist" + to_string(start);
  graph.accessNode(startKey, 1, {0, start});

  Priority_queue *pq = new Priority_queue();
  pair<int, int> startVal = {0, start};
  pq->insert(startVal);

  if (debugMode)
    cout << "\nDijkstra Traversal:" << endl;

  bool innerLoop = false;
  int u = 0, d = 0;
  int ct = 1;
  for (int i = 0; i < 2 * V + E; i++)
  {
    if (!innerLoop)
    {
      auto top = pq->extract_min();
      if (debugMode)
        cout << "Extracted vertex " << top.second << " with distance " << top.first << endl;
      u = top.second;
      d = top.first;
      string uKey = "Dist" + to_string(u);
      pair<int, int> uVal = graph.accessNode(uKey, 0, {0, 0});
      if (d == uVal.first)
      {
        string uVisKey = "visited" + to_string(u);
        graph.accessNode(uVisKey, 1, {1, 1});
      }
      innerLoop = true;
    }
    else
    {
      string neighborKey = "Eout" + to_string(u) + "_" + to_string(ct);
      if (graph.contains(neighborKey))
      {
        pair<int, int> neighborVal = graph.accessNode(neighborKey, 0, {0, 0});
        int v = neighborVal.first;
        int w = neighborVal.second;
        string vKey = "Dist" + to_string(v);
        pair<int, int> vVal = graph.accessNode(vKey, 0, {0, 0});
        if (d + w < vVal.first)
        {
          pair<int, int> newVal = {d + w, v};
          if (debugMode)
            cout << "Updating vertex " << v << " with new distance " << newVal.first << endl;
          pq->insert(newVal);
          graph.accessNode(vKey, 1, newVal);
        }
        ct++;
      }
      else
      {
        innerLoop = false;
        ct = 1;
      }
    }
  }

  if (debugMode)
  {
    // Output final distance array.
    cout << "\nFinal distances:" << endl;
    for (int i = 1; i <= V; i++)
    {
      string key = "Dist" + to_string(i);
      int dist = graph.accessNode(key, 0, {0, 0}).first;
      cout << "Vertex " << i << ": " << dist << endl;
    }
  }

  delete pq;
}

void GraphProcessing::dfs(domap graph, int src)
{
  if (debugMode)
    cout << "\nDFS Traversal:" << endl;

  int stkCt = 0;
  // Push the source onto the stack with an initial counter of 1.
  string stkKey = "InStk" + to_string(stkCt);
  graph.addEdgeByString(stkKey, {src, 1});

  // Mark src as visited.
  string visKey = "visited" + to_string(src);
  if (!graph.contains(visKey))
    graph.addEdgeByString(visKey, {1, 1});
  else
    graph.accessNode(visKey, 1, {1, 1});
  if (debugMode)
    cout << "Visited: " << src << endl;
  stkCt++;

  while (stkCt > 0)
  {
    string topKey = "InStk" + to_string(stkCt - 1);
    auto top = graph.accessNode(topKey, 0, {0, 0});
    int u = top.first;
    int cnt = top.second;
    string nextKey = "Eout" + to_string(u) + "_" + to_string(cnt);

    if (graph.contains(nextKey))
    {
      auto nextVal = graph.accessNode(nextKey, 0, {0, 0});
      int v = nextVal.first;
      string vKey = "visited" + to_string(v);
      if (!graph.contains(vKey))
      {
        // Update the counter on the current stack element.
        graph.addEdgeByString(topKey, {u, cnt + 1});
        // Push the neighbor v onto the stack.
        string newStackKey = "InStk" + to_string(stkCt);
        graph.addEdgeByString(newStackKey, {v, 1});
        stkCt++;
        if (debugMode)
          cout << "Visited: " << v << endl;
        // Mark v as visited.
        if (!graph.contains(vKey))
          graph.addEdgeByString(vKey, {1, 1});
        else
          graph.accessNode(vKey, 1, {1, 1});
      }
      else
      {
        // If already visited, update the counter to check the next edge.
        graph.addEdgeByString(topKey, {u, cnt + 1});
      }
    }
    else
    {
      // No more edges for the current vertex; pop from stack.
      stkCt--;
    }
  }
}
