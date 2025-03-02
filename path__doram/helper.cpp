#include <cstdlib>
#include <random>
using namespace std;
int generateRandomNumber(int l, int r)
{
  // Use static to initialize the random engine only once
  static random_device rd;
  static mt19937 gen(rd());
  uniform_int_distribution<> distrib(l, r);
  return distrib(gen);
}