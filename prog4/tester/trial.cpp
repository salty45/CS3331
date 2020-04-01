#include "trial.h"

int Trial::add(int m, int n)
{
   return n + m;
}

int Trial::sub(int m, int n)
{
    return m - n;
}

double Trial:: div(double m, double n)
{
    return m / n;
}

double Trial::avg(int *arr, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    return div(sum, len);
}
    

int main(void)
{

    Trial *t = new Trial();
    int s = t->sub(3, 2);
    int arr[2] = {1, 4};
    double d = t->avg(arr, 2);
    printf("s = %d  avg = %f\n", s, d);
    return 0;
}
