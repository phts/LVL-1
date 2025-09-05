#ifndef utils_h
#define utils_h

class Utils
{
public:
  double average(double arr[], byte len)
  {
    double sum = 0;
    for (int i = 0; i < len; i++)
    {
      sum = sum + arr[i];
    }
    return sum / len;
  }

  void sort(double arr[], int len)
  {
    double tmp;
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

  double *subarray(double arr[], int from, int to)
  {
    int len = to - from;
    double *newArr = new double[len];
    memcpy(newArr, &arr[from], len * sizeof(double));
    return newArr;
  }
};
Utils utils;

#endif
