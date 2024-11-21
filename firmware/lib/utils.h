#ifndef utils_h
#define utils_h

#include "console.h"

class Utils
{
public:
  float average(float arr[], byte len)
  {
    float sum = 0;
    for (int i = 0; i < len; i++)
    {
      sum = sum + arr[i];
    }
    return sum / len;
  }

  void sort(float arr[], int len)
  {
    float tmp;
    for (int i = 0; i < len; i++)
    {
      for (int j = i + 1; j < len; j++)
      {
        if (arr[i] > arr[j])
        {
          tmp = arr[i];
          arr[i] = arr[j];
          arr[j] = tmp;
        }
      }
    }
  }

  float *subarray(float arr[], int from, int to)
  {
    int len = to - from;
    float *newArr = new float[len];
    memcpy(newArr, &arr[from], len * sizeof(float));
    return newArr;
  }
};
Utils utils;

#endif
