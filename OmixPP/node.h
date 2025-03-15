#pragma once
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

class node
{
public:
  string key;           //
  pair<int, int> value; //
  int height;           //
  int leftHeight;       //
  int rightHeight;      //
  string leftKey;       //
  string rightKey;      //
  int pos;              //
  int leftPos;          //
  int rightPos;         //
  int isDummy;          //
  int level;            //

  // default constructor
  node()
      : key(""), value({0, 0}), height(0), leftHeight(0), rightHeight(0),
        leftKey(""), rightKey(""), pos(0), leftPos(-1), rightPos(-1), isDummy(0), level(0)
  {
  }

  node(string key, pair<int, int> value, int height, int leftHeight, int rightHeight, string leftKey, string rightKey, int pos, int leftPos, int rightPos, int isDummy, int level)
  {
    this->key = key;
    this->value = value;
    this->height = height;
    this->leftHeight = leftHeight;
    this->rightHeight = rightHeight;
    this->leftKey = leftKey;
    this->rightKey = rightKey;
    this->pos = pos;
    this->leftPos = leftPos;
    this->rightPos = rightPos;
    this->isDummy = isDummy;
    this->level = 0;
  }

  // to print the node => just for debugging
  void printNode()
  {
    // print all fields of the node
    // print in diff lines
    cout << "Key: " << key << endl;
    cout << "Value: {" << value.first << "," << value.second << "}" << endl;
    cout << "Height: " << height << endl;
    cout << "Left Height: " << leftHeight << endl;
    cout << "Right Height: " << rightHeight << endl;
    cout << "Left Key: " << leftKey << endl;
    cout << "Right Key: " << rightKey << endl;
    cout << "Position: " << pos << endl;
    cout << "Left Position: " << leftPos << endl;
    cout << "Right Position: " << rightPos << endl;
    cout << "Is Dummy: " << isDummy << endl;
    cout << "Level: " << level << endl;
  }
};