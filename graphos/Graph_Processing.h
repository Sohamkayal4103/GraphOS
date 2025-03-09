#include "DOMAP.h"
using namespace std;

class GraphProcessing
{
public:
  void bfs(DOMAP *graph, int start);
  void dijkstra(int V, int E, DOMAP *graph, int start);
  void dfs(DOMAP *graph, int start);
};