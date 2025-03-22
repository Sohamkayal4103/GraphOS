#include "../OmixPP/domap.h"
using namespace std;

class GraphProcessing
{
public:
  bool debugMode = false;
  void bfs(domap graph, int start);
  void dfs(domap graph, int src);
  void dijkstra(int V, int E, domap graph, int start);
  void setDebugMode(bool mode);
};