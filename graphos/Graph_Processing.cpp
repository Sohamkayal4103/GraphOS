#include "Graph_Processing.h"
#include "Priority_queue.h"
#include <iostream>
using namespace std;

void GraphProcessing::bfs(domap graph, int start)
{
  int source = start;
  int qCnt = 0, curQcnt = 0;
  bool outerL = true;
  string key = "InQ" + to_string(qCnt);
  pair<int, int> val = {start, 0};
  graph.addEdgeByString(key, val);

  // graph->printQ();

  // first check if that key exists
  string visKey = "visited" + to_string(start);
  if (!graph.contains(visKey))
  {
    cout << "Inserting visited node: " << visKey << endl;
    graph.addEdgeByString(visKey, {1, 1});
  }
  graph.accessNode(visKey, 1, {1, 1});
  qCnt++;
  int cnt = 1;
  bool visited = true;
  while (qCnt != curQcnt)
  {
    string qKey = "InQ" + to_string(curQcnt);
    int tmp = graph.accessNode(qKey, 0, {0, 0}).first;
    if (outerL)
    {
      cout << "Popping out: " << tmp << endl;
      if (tmp == 0)
      {
        cout << "not found in graph" << qKey << endl;
      }
      cout << "Qcnt: " << qCnt << " curQcnt: " << curQcnt << endl;
      source = tmp;
      curQcnt++;
      cnt = 1;
    }
    string nextKey = "Eout" + to_string(source) + "_" + to_string(cnt);
    cout << "Checking for next edge at: " << nextKey << endl;
    if (graph.contains(nextKey))
    {
      outerL = false;
      int trm = graph.accessNode(nextKey, 0, {0, 0}).first;
      // cout << "Found next node: " << trm << endl;
      string nextVisKey = "visited" + to_string(trm);
      visited = graph.contains(nextVisKey);
      bool mostInner = !visited & !outerL;
      // just for debugging
      if (!visited)
      {
        cout << "Next node is not visited: " << trm << endl;
      }
      else
      {
        cout << "Next node is visited: " << trm << endl;
      }
      int nextTmp;
      if (mostInner)
      {
        nextTmp = trm;
      }
      else
      {
        nextTmp = -1;
      }
      string nextQKey = "InQ" + to_string(qCnt);
      pair<int, int> nextVal = {nextTmp, 0};
      graph.addEdgeByString(nextQKey, nextVal);
      cout << "Inserted: " << nextQKey << " with value: " << nextTmp << endl;
      // graph.printQ();
      if (mostInner)
      {
        if (!graph.contains(nextVisKey))
        {
          cout << "Inserting visited node: " << nextVisKey << endl;
          graph.addEdgeByString(nextVisKey, {1, 1});
        }
        else
        {
          // key present => just update
          cout << "Updating visited node: " << nextVisKey << endl;
          graph.accessNode(nextVisKey, 1, {1, 1});
        }
        // graph.accessNode(nextVisKey, 1, {1, 1});
        // cout << "Marked as visited: " << nextVisKey << endl;
        qCnt++;
        cout << "Incremented qCnt: " << qCnt << endl;
      }
      if (!outerL)
      {
        cnt++;
        // cout << "Incremented cnt: " << cnt << endl;
      }
      cout << "--------------------------" << endl;
      cout << endl;
      cout << endl;
      cout << endl;
    }
    else
    {
      cout << "No next edge found" << endl;
      cout << "setting outerL to true" << endl;
      cout << "--------------------------" << endl;
      cout << endl;
      cout << endl;
      cout << endl;
      outerL = true;
    }
  }
}

void GraphProcessing::dijkstra(int V, int E, domap graph, int start)
{
  Priority_queue *pq = new Priority_queue();
  for (int i = 1; i <= V; i++)
  {
    string key = "Dist" + to_string(i);
    pair<int, int> val = {INT_MAX, 0};
    graph.addEdgeByString(key, val);
  }
  pair<int, int> startVal = {0, start}; // dist , node
  string startKey = "Dist" + to_string(start);
  graph.accessNode(startKey, 1, startVal);
  pq->insert(startVal);
  bool innerLoop = false;
  int u = 0, d = 0;
  int ct = 1;
  for (int i = 0; i < 2 * V + E; i++)
    if (!innerLoop)
    {
      pair<int, int> top = pq->extract_min();
      cout << "Extracted: " << top.first << " " << top.second << endl;
      u = top.second;
      d = top.first;                       // dist
      string uKey = "Dist" + to_string(u); // dist[u]
      cout << "uKey: " << uKey << endl;
      cout << "d: " << d << endl;
      cout << "u: " << u << endl;
      cout << "--------------------------" << endl;
      pair<int, int> uVal = graph.accessNode(uKey, 0, {0, 0});
      int du = uVal.first;
      if (d == du)
      {
        string uVisKey = "visited" + to_string(u);
        // graph->markVis(uVisKey);
        graph.accessNode(uVisKey, 1, {1, 1});
        // loop invariant - any distance and vertex pair {d,u} after this will be greate than current d
      }
      innerLoop = true;
    }
    else
    {
      string neighborKey = "Eout" + to_string(u) + "_" + to_string(ct);
      cout << "Checking for neighbor: " << neighborKey << endl;
      // cout << "d: " << d << endl;
      // cout << "u: " << u << endl;
      // cout << "ct: " << ct << endl;
      cout << "--------------------------" << endl;
      if (graph.contains(neighborKey))
      {
        pair<int, int> neighborVal = graph.accessNode(neighborKey, 0, {0, 0});
        cout << "Found neighbor: " << neighborVal.first << endl;
        cout << "Weight: " << neighborVal.second << endl;
        int v = neighborVal.first;
        string vKey = "Dist" + to_string(v);
        pair<int, int> vVal = graph.accessNode(vKey, 0, {0, 0});
        int dv = vVal.first; // dist[v]
        int w = neighborVal.second;
        cout << "d: " << d << endl;
        cout << "w: " << w << endl;
        cout << "dv: " << dv << endl;
        cout << "--------------------------" << endl;
        if (d + w < dv)
        {
          pair<int, int> newVal = {d + w, v};
          cout << "Inserting New Value: " << newVal.first << " " << newVal.second << endl;
          cout << "--------------------------" << endl;
          cout << endl;
          pq->insert(newVal);
          pair<int, int> graphVal = {d + w, v}; // dist, node
          cout << "Updating graph: " << vKey << " to " << graphVal.first << " " << graphVal.second << endl;
          graph.accessNode(vKey, 1, graphVal);
        }
        ct++;
      }
      else
      {
        cout << "No neighbor found" << endl;
        innerLoop = false;
        ct = 1;
        cout << "Resetting ct: " << ct << endl;
        cout << "--------------------------" << endl;
        cout << endl;
      }
    }
}

void GraphProcessing::dfs(domap graph, int src)
{

  int stkCt = 0;
  string key = "InStk" + to_string(stkCt);
  pair<int, int> val = {src, 1};
  cout << "Inserting: " << key << endl;
  cout << "Value: " << val.first << " " << val.second << endl;
  graph.addEdgeByString(key, val);
  string visKey = "visited" + to_string(src);
  // graph-markVis(visKey);
  if (!graph.contains(visKey))
  {
    cout << "Inserting visited node: " << visKey << endl;
    graph.addEdgeByString(visKey, {1, 1});
  }
  else
  {
    // key present => just update
    cout << "Updating visited node: " << visKey << endl;
    graph.accessNode(visKey, 1, {1, 1});
  }
  cout << "Marked as visited: " << visKey << endl;
  stkCt++;
  while (stkCt > 0)
  {
    string stkKey = "InStk" + to_string(stkCt - 1);
    pair<int, int> top = graph.accessNode(stkKey, 0, {0, 0});
    cout << "Popping out: " << top.first << endl;
    if (top.first == 0)
    {
      cout << "not found in graph" << stkKey << endl;
    }
    int u = top.first;
    int cnt = top.second;
    string nextKey = "Eout" + to_string(u) + "_" + to_string(cnt);
    cout << "Checking for next node at: " << nextKey << endl;
    if (graph.contains(nextKey))
    {
      pair<int, int> nextVal = graph.accessNode(nextKey, 0, {0, 0});
      cout << "Found next edge: " << nextVal.first << endl;
      int v = nextVal.first;
      cout << "Found next node: " << v << endl;
      string vKey = "visited" + to_string(v);
      bool visited = graph.contains(vKey);
      if (!visited)
      {
        // increase the count of the edge at the top of the stack
        pair<int, int> newTop = {u, cnt + 1};
        // graph->update(stkKey, newTop);
        graph.accessNode(stkKey, 1, newTop);
        cout << "Updating top of stack: " << stkKey << " to " << newTop.first << " " << newTop.second << endl;
        // push the new vertex to the stack
        string nextStkKey = "InStk" + to_string(stkCt);
        cout << "Inserting at : " << nextStkKey << endl;
        pair<int, int> nextStkVal = {v, 1};
        cout << "Value: " << nextStkVal.first << " " << nextStkVal.second << endl;
        // graph->insertQ(nextStkKey, nextStkVal);
        graph.addEdgeByString(nextStkKey, nextStkVal);
        stkCt++;

        // mark the vertex as visited
        // graph->markVis(vKey);
        if (!graph.contains(vKey))
        {
          cout << "Inserting visited node: " << vKey << endl;
          graph.addEdgeByString(vKey, {1, 1});
        }
        else
        {
          // key present => just update
          cout << "Updating visited node: " << vKey << endl;
          graph.accessNode(vKey, 1, {1, 1});
        }
        cout << "Marked as visited: " << vKey << endl;
      }
      else
      {
        // increase the count of the edge
        pair<int, int> newTop = {u, cnt + 1};
        // graph->update(stkKey, newTop);
        graph.accessNode(stkKey, 1, newTop);
        cout << "Updating top of stack: " << stkKey << " to " << newTop.first << " " << newTop.second << endl;
        cnt++;
      }
    }
    else
    {
      // pop the vertex from the stack
      cout << "Popping: " << stkKey << " " << top.first << " " << top.second << endl;
      stkCt--;
    }
  }
}
