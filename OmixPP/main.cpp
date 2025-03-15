#include "domap.h"

using namespace std;

int main()
{

  vector<vector<int>> graphData = {{1, 2, 3}, {2, 4, 1}};

  // creating a domap object
  domap omixpp = domap(graphData, 4);

  // print the state of the tree
  omixpp.printState();

  // access the node
  string key = "Eout1_1";
  pair<int, int> ans = omixpp.accessNode(key, 0, {0, 0});
  cout << "Accessed node value: {" << ans.first << "," << ans.second << "}" << endl;
  // omixpp.printState();
  string s1;
  while (true)
  {
    cin >> s1;
    if (s1 == "z")
    {
      break;
    }
    cout << "You entered: " << s1 << endl;
    pair<int, int> ans = omixpp.accessNode(s1, 0, {0, 0});
    if (ans.first == 0 && ans.second == 0)
    {
      cout << "Node not found" << endl;
      continue;
    }
    cout << "Accessed node value: {" << ans.first << "," << ans.second << "}" << endl;
  }

  // add an edge
  // omixpp.addEdge(1, 3, 2);
  omixpp.addEdgeByString("Eout1_2", {3, 2});

  // update an edge
  string edgeToUpdate = "Eout1_1";
  omixpp.accessNode(edgeToUpdate, 1, {0, 1});

  while (true)
  {
    cin >> s1;
    if (s1 == "z")
    {
      break;
    }
    cout << "You entered: " << s1 << endl;
    pair<int, int> ans = omixpp.accessNode(s1, 0, {0, 0});
    if (ans.first == 0 && ans.second == 0)
    {
      cout << "Node not found" << endl;
      continue;
    }
    cout << "Accessed node value: {" << ans.first << "," << ans.second << "}" << endl;
  }

  return 0;
}