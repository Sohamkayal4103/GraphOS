#include "helper.h"
#include "node.h"
int helper::generateRandomNumber(int l, int r)
{
  // Use static to initialize the random engine only once
  static random_device rd;
  static mt19937 gen(rd());
  uniform_int_distribution<> distrib(l, r);
  return distrib(gen);
}