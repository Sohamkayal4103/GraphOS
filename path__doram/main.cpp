#include "doram.h"
#include "stash.h"
#include <iostream>
using namespace std;

int main()
{
  cout << "Hello World" << endl;
  vector<vector<int>> keyValues = {{10, 1}, {20, 2}, {30, 9}, {40, 4}, {50, 5}, {60, 6}, {70, 7}};

  // Intialized the DORAM
  vector<int> ans;
  DORAM *tr = new DORAM(keyValues.size());
  tr->initialize(keyValues);

  // Access the key 30
  ans.push_back(tr->access(30, 1, 99));
  // ans.push_back(tr->access(20, 2, 100));
  ans.push_back(tr->access(50, 1, 99));
  ans.push_back(tr->access(20, 1, 99));
  ans.push_back(tr->access(10, 1, 99));
  ans.push_back(tr->access(70, 1, 99));
  ans.push_back(tr->access(31, 1, 99));
  ans.push_back(tr->access(70, 2, 99));
  ans.push_back(tr->access(70, 1, 99));
  ans.push_back(tr->access(60, 1, 99));
  ans.push_back(tr->access(40, 1, 99));

  cout << "-------------------" << endl;
  cout << "Final Answers from DORAM: " << endl;
  cout << "-------------------" << endl;

  for (int i = 0; i < (int)ans.size(); i++)
  {
    cout << "Answer: " << ans[i] << endl;
  }
  cout << "-------------------" << endl;

  return 0;
}