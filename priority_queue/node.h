#pragma once
#include <iostream>
using namespace std;

class node
{
public:
  int key;
  int value;
  int pos;
  int height;

  int leftPos;
  int leftKey;
  int letHeight;

  int rightPos;
  int rightKey;
  int rightHeight;

  int subtreeMin;
  int leftTreeMin;
  int rightTreeMin;

  int isDummy = 0;

  node(int _key, int _value, int _pos, int _height, int _leftPos, int _leftKey, int _letHeight, int _rightPos, int _rightKey, int _rightHeight, int _subtreeMin, int _leftTreeMin, int _rightTreeMin, bool _isDummy)
  {
    key = _key;
    value = _value;
    pos = _pos;
    height = _height;
    leftPos = _leftPos;
    leftKey = _leftKey;
    letHeight = _letHeight;
    rightPos = _rightPos;
    rightKey = _rightKey;
    rightHeight = _rightHeight;
    subtreeMin = _subtreeMin;
    leftTreeMin = _leftTreeMin;
    rightTreeMin = _rightTreeMin;
    isDummy = _isDummy;
  }
};