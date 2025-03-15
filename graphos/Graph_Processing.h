#include "../OmixPP/domap.h"
using namespace std;

class GraphProcessing
{
public:
  void bfs(domap graph, int start);
  void dfs(domap graph, int src);
  void dijkstra(int V, int E, domap graph, int start);
};