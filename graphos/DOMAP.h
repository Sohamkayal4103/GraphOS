#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class DOMAP
{
public:
  unordered_map<string, pair<int, int>> mpp;
  DOMAP(vector<vector<int>> &data);
  void insert(vector<int> &data);
  void insertQ(string &key, pair<int, int> &value);
  void insertDist(string &key, pair<int, int> value);
  void markVis(string &key);
  void remove(vector<int> &data);
  void print();
  void printStk();
  void printQ();
  void printDist();
  void printVis();
  pair<int, int> access(string &key);
  bool contains(string &key);
  void clear();
  int size();
  bool empty();
  void update(string key, pair<int, int> &value);
};