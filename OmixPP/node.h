#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <utility>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>

using namespace std;

class node
{
public:
  string key;           // Node key
  pair<int, int> value; // Value pair
  int height;           //
  int leftHeight;       //
  int rightHeight;      //
  string leftKey;       //
  string rightKey;      //
  int pos;              //
  int leftPos;          //
  int rightPos;         //
  int isDummy;          // 1 if dummy, 0 otherwise
  int level;            //

  // Default constructor
  node()
      : key(""), value({0, 0}), height(0), leftHeight(0), rightHeight(0),
        leftKey(""), rightKey(""), pos(0), leftPos(-1), rightPos(-1), isDummy(0), level(0)
  {
  }

  // Parameterized constructor
  node(string key, pair<int, int> value, int height, int leftHeight, int rightHeight,
       string leftKey, string rightKey, int pos, int leftPos, int rightPos, int isDummy, int level)
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
    this->level = level;
  }

  // For debugging: print the node
  void printNode()
  {
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

private:
  // Boost Serialization support
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int /*version*/)
  {
    ar & key;
    ar & value;
    ar & height;
    ar & leftHeight;
    ar & rightHeight;
    ar & leftKey;
    ar & rightKey;
    ar & pos;
    ar & leftPos;
    ar & rightPos;
    ar & isDummy;
    ar & level;
  }
};

#endif // NODE_H
