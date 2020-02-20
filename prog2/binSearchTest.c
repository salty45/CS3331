#include <stdio.h>
#include <stdlib.h>

long linSearch(long i, long *x, long ix, long *y, long iy)
{
    long k = 0;
    while (k < iy && x[i] > y[k])
    {
        k++;
    }
    return k + i;
}

long diff(long a, long b)
{
    if ((a - b) < 0)
        return b - a;
    else
        return a - b;   
}

long binSearch(long i, long *x, long ix, long *y, long iy)
{
    long k = 0;
    long j = iy - 1;
    long m = (j - k)/2 + 1;
    int n = 0;
    if (x[i] < y[0])
        return i;
    if (x[i] > y[iy - 1])
        return i + iy;
    printf("%ld  %ld  i = %ld\n", x[i], y[iy-1], i);
    printf("k = %ld  j = %ld  m = %ld\n", k, j, m);
    while (k < j)
    {
        if (x[i] < y[m])
        {
            /* Guessed too high */
            j = m - 1;
            m = (j - k)/2 + 1;
        } else {
            /* Guessed too low */
            k = m + 1;
            m = m + (j - k)/2 + 1;
        }
/*        m = (j - k)/2;*/
        printf("k: %d  j: %d  m: %d\n", k, j, m);
 /*       if (diff(k, j) < 1)
            break;
        if (n++ > 12)
            break;*/
    }
    printf("k = %ld  i = %ld\n", k, i);
    return k + i;
}


int main(void)
{
    long x1[5] = {1, 2, 3, 6, 9};
    long y1[6] = {-1, 4, 5, 7, 8, 10 };
    int i = 0;
    long r = 0;
    for (i = 0; i < 6; i++)
    {
        r = linSearch(i, y1, 6, x1, 5);
        printf("x[%d] goes in spot %d\n", i, r);
    }
    for (i = 0; i < 6; i++)
    {
        r = binSearch(i, y1, 6, x1, 5);
        printf("x[%d] goes in spot %d\n", i, r);
    }


    return 0;
}
