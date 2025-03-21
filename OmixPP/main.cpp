#include "domap.h"
#include <chrono>

using namespace std;

int main()
{

  vector<vector<int>> graphData = {{1, 2, 3}, {2, 4, 1}};

  // creating a domap object
  auto initTime = chrono::high_resolution_clock::now();
  domap omixpp = domap(graphData, 4);
  auto initEndTime = chrono::high_resolution_clock::now();

  auto initDuration = chrono::duration_cast<chrono::microseconds>(initEndTime - initTime);
  // //cout << "Initialization time: " << initDuration.count() << " microseconds" << endl;

  // print the state of the tree
  omixpp.printState();

  // access the node
  string key = "Eout1_1";
  auto start = chrono::high_resolution_clock::now();
  pair<int, int> ans = omixpp.accessNode(key, 0, {0, 0});
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  // //cout << "Time taken: " << duration.count() << " microseconds" << endl;
  // //cout << "Accessed node value: {" << ans.first << "," << ans.second << "}" << endl;

  // Loop to measure multiple calls and compute the average duration
  string s1;
  long long totalDuration = 0; // Sum of durations in microseconds
  int count = 0;               // Count of iterations

  while (true)
  {
    // //cout << "\nEnter a key (or 'z' to exit): ";
    cin >> s1;
    if (s1 == "z")
    {
      break;
    }

    start = chrono::high_resolution_clock::now();
    pair<int, int> ans = omixpp.accessNode(s1, 0, {0, 0});
    end = chrono::high_resolution_clock::now();
    auto iterDuration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    totalDuration += iterDuration;
    count++;

    // //cout << "You entered: " << s1 << endl;
    if (ans.first == 0 && ans.second == 0)
    {
      // //cout << "Node not found" << endl;
    }
    else
    {
      // //cout << "Accessed node value: {" << ans.first << "," << ans.second << "}" << endl;
    }
    // //cout << "Time taken: " << iterDuration << " microseconds" << endl;
  }

  // Display the average time if at least one iteration was processed
  if (count > 0)
  {
    double average = static_cast<double>(totalDuration) / count;
    // //cout << "\nAverage time taken: " << average << " microseconds over " << count << " iterations." << endl;
  }
  else
  {
    // //cout << "\nNo iterations were processed." << endl;
  }

  // add an edge
  // omixpp.addEdge(1, 3, 2);
  string insKey;
  int first, second;
  long long totalInsertionDuration = 0;
  int insertionCount = 0;

  while (true)
  {
    // //cout << "\nEnter a key for insertion (or 'z' to exit): ";
    cin >> insKey;
    if (insKey == "z")
    {
      break;
    }
    // //cout << "Enter two integers for the value (separated by a space): ";
    cin >> first >> second;

    auto insStart = chrono::high_resolution_clock::now();
    omixpp.addEdgeByString(insKey, {first, second});
    auto insEnd = chrono::high_resolution_clock::now();
    auto insertionDuration = chrono::duration_cast<chrono::microseconds>(insEnd - insStart).count();

    totalInsertionDuration += insertionDuration;
    insertionCount++;

    // //cout << "Inserted key: " << insKey << " with value: {" << first << "," << second << "}" << endl;
    // //cout << "Time taken for insertion: " << insertionDuration << " microseconds" << endl;
  }

  if (insertionCount > 0)
  {
    double averageInsertion = static_cast<double>(totalInsertionDuration) / insertionCount;
    // //cout << "\nAverage insertion time: " << averageInsertion
    //   << " microseconds over " << insertionCount << " iterations." << endl;
  }
  else
  {
    // //cout << "\nNo insertion iterations were processed." << endl;
  }

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
    // //cout << "You entered: " << s1 << endl;
    pair<int, int> ans = omixpp.accessNode(s1, 0, {0, 0});
    if (ans.first == 0 && ans.second == 0)
    {
      // //cout << "Node not found" << endl;
      continue;
    }
    // //cout << "Accessed node value: {" << ans.first << "," << ans.second << "}" << endl;
  }

  return 0;
}