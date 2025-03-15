#pragma once
#include "node.h"
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

class helper
{
public:
  int calculateLowestPossibleLevel(int currBucketId, int assignedBucketId, int treeHeight);
  int findIntersectingBucket(int a, int b);
  int generateRandomNumber(int l, int r);

  // Template function defined in the header.
  template <typename T, typename Compare>
  void bitonicSort(vector<T> &arr, int low, int cnt, bool ascending, Compare comp)
  {
    if (cnt > 1)
    {
      int k = cnt / 2;
      // First half in ascending order.
      bitonicSort(arr, low, k, true, comp);
      // Second half in descending order.
      bitonicSort(arr, low + k, k, false, comp);
      // Merge both halves.
      bitonicMerge(arr, low, cnt, ascending, comp);
    }
  }

private:
  // Private helper template function for merging.
  template <typename T, typename Compare>
  void bitonicMerge(vector<T> &arr, int low, int cnt, bool ascending, Compare comp)
  {
    if (cnt > 1)
    {
      int k = cnt / 2;
      for (int i = low; i < low + k; i++)
      {
        if (ascending)
        {
          if (comp(arr[i + k], arr[i]))
          {
            swap(arr[i], arr[i + k]);
          }
        }
        else
        {
          if (comp(arr[i], arr[i + k]))
          {
            swap(arr[i], arr[i + k]);
          }
        }
      }
      bitonicMerge(arr, low, k, ascending, comp);
      bitonicMerge(arr, low + k, k, ascending, comp);
    }
  }
};
