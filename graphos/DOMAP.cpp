#include "DOMAP.h"
using namespace std;

DOMAP::DOMAP(vector<vector<int>> &data)
{
  mpp = unordered_map<string, pair<int, int>>();
  for (int i = 0; i < data.size(); i++)
  {
    int u = data[i][0];
    int v = data[i][1];
    int w = data[i][2];
    string k1 = "V" + to_string(u);
    string k2 = "V" + to_string(v);
    // updating dOut for u
    mpp[k1].second++;
    // updating dIn for v
    mpp[k2].first++;

    string k3 = "Eout" + to_string(u) + "_" + to_string(mpp[k1].second);
    string k4 = "Ein" + to_string(v) + "_" + to_string(mpp[k2].first);
    mpp[k3] = {v, w};
    mpp[k4] = {u, w};
  }
}

void DOMAP::insert(vector<int> &data)
{
  int u = data[0];
  int v = data[1];
  int w = data[2];
  string k1 = "V" + to_string(u);
  string k2 = "V" + to_string(v);
  // updating dOut for u
  mpp[k1].second++;
  // updating dIn for v
  mpp[k2].first++;

  string k3 = "Eout" + to_string(u) + "_" + to_string(mpp[k1].second);
  string k4 = "Ein" + to_string(v) + "_" + to_string(mpp[k2].first);
  mpp[k3] = {v, w};
  mpp[k4] = {u, w};
}

void DOMAP::remove(vector<int> &data)
{
  int u = data[0];
  int v = data[1];
  string k1 = "V" + to_string(u);
  string k2 = "V" + to_string(v);
  // updating dOut for u
  mpp[k1].second--;
  // updating dIn for v
  mpp[k2].first--;

  string k3 = "Eout" + to_string(u) + "_" + to_string(mpp[k1].second + 1);
  string k4 = "Ein" + to_string(v) + "_" + to_string(mpp[k2].first + 1);
  mpp.erase(k3);
  mpp.erase(k4);
}

void DOMAP::insertQ(string &key, pair<int, int> &value)
{
  mpp[key] = value;
}

void DOMAP::insertDist(string &key, pair<int, int> value)
{
  mpp[key] = value;
}

void DOMAP::markVis(string &key)
{
  mpp[key].first = 1;
  mpp[key].second = 1;
}

void DOMAP::print()
{
  cout << "Printing DOMAP" << endl;
  cout << "---------------------" << endl;
  for (auto it = mpp.begin(); it != mpp.end(); it++)
  {
    cout << it->first << " " << it->second.first << " " << it->second.second << endl;
  }
  cout << "---------------------" << endl;
}

void DOMAP::printStk()
{
  cout << "Printing Stack" << endl;
  cout << "---------------------" << endl;
  for (auto it = mpp.begin(); it != mpp.end(); it++)
  {
    if (it->first.find("InStk") != string::npos)
      cout << it->first << " " << it->second.first << " " << it->second.second << endl;
  }
  cout << "---------------------" << endl;
}
void DOMAP::printQ()
{
  cout << "Printing Queue" << endl;
  cout << "---------------------" << endl;
  for (auto it = mpp.begin(); it != mpp.end(); it++)
  {
    if (it->first.find("InQ") != string::npos)
      cout << it->first << " " << it->second.first << " " << it->second.second << endl;
  }
  cout << "---------------------" << endl;
}
void DOMAP::printDist()
{
  cout << "Printing Distances" << endl;
  cout << "---------------------" << endl;
  for (auto it = mpp.begin(); it != mpp.end(); it++)
  {
    if (it->first.find("Dist") != string::npos)
      cout << it->first << " " << it->second.first << " " << it->second.second << endl;
  }
  cout << "---------------------" << endl;
}
void DOMAP::printVis()
{
  cout << "Printing Visited" << endl;
  cout << "---------------------" << endl;
  for (auto it = mpp.begin(); it != mpp.end(); it++)
  {
    if (it->first.find("visited") != string::npos)
      cout << it->first << " " << it->second.first << " " << it->second.second << endl;
  }
  cout << "---------------------" << endl;
}

pair<int, int> DOMAP::access(string &key)
{
  return mpp[key];
}

bool DOMAP::contains(string &key)
{
  return mpp.find(key) != mpp.end();
}

void DOMAP::clear()
{
  mpp.clear();
}

int DOMAP::size()
{
  return mpp.size();
}

bool DOMAP::empty()
{
  return mpp.empty();
}

void DOMAP::update(string key, pair<int, int> &value)
{
  mpp[key] = value;
}