#ifndef utils_h
#define utils_h

class Utils
{
public:
  unsigned long average(unsigned long arr[], byte len)
  {
    unsigned long sum = 0;
    for (int i = 0; i < len; i++)
    {
      sum = sum + arr[i];
    }
    return sum / len;
  }

  void sort(unsigned long arr[], int len)
  {
    unsigned long tmp;
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

  unsigned long *subarray(unsigned long arr[], int from, int to)
  {
    int len = to - from;
    unsigned long *newArr = new unsigned long[len];
    memcpy(newArr, &arr[from], len * sizeof(unsigned long));
    return newArr;
  }
};
Utils utils;

#endif
