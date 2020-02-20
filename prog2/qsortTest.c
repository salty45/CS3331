#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void swap(long *arr, long a, long b)
{
    long x = arr[a];
    arr[a] = arr[b];
    arr[b] = x;
}


int lomuto(long *arr, long l, long r)
{
    long p = arr[l];
    long si = l;
    long i = 0;
    for (i = l + 1; i <= r; i++)
    {
        if (arr[i] < p)
        {
            si++;
            swap(arr, si, i);
        }
    }
    swap(arr, l, si);
    return si;
}


int partition(long *arr, long l, long r)
{
    return lomuto(arr, l, r);
    long p = arr[l];
    long i = l;
    long j = r + 1;
    
    do 
    {
        printf("%s\n", "top of do loop");
        do
        {
            i++;
            printf("%s\n", "in i");
        } while (arr[i] < p);
        printf("%s\n", "after i loop");
        do 
        {
            printf("%s\n", "in j");
            j--;
        } while (arr[j] > p);
        swap(arr, i, j);      
        printf("after swap%s\n", ""); 
    } while (i < j);
    /*while (i <= j)
    {
        while (arr[i] <= p)
            i++;
        while (arr[j] >= p)
            j--;   
        swap(arr, i, j);
    }*/
    swap(arr, i, j);
    swap(arr, l, j);
    return j;
}


void qsortr(long *arr, long n, long l, long r)
{
    long pivot = partition(arr, l, r);
    /*printf("Pivot: %ld\n", pivot);*/
    if (l < r)
    {
        qsortr(arr, n, l, pivot - 1);
        qsortr(arr, n, pivot + 1, r);
    }
}

/* Second array expected values, 1st actual vals */
void testEquals(long *arr1, long *arr2, long len)
{
    long i = 0;
    long errs = 0;
    for (i = 0; i < len; i++)
    {
        if (arr1[i] != arr2[i])
        {
            printf("Failure of sort at index %d: exp: %d but was %d\n", i,
                arr2[i], arr1[i]);
            errs++;
            printArr(arr1, len);
            break;
        }
    }
    if (errs == 0)
        printf("%s\n", "Perfect sort!");
}

void printArr(long *arr, long len)
{
    long i = 0;
    printf("%s\n{ ", "Printing array:");
    for (i = 0; i < len; i++)
    {
        printf("%ld, ", arr[i]);
    }
    printf(" %s\n\n", "}");
}

int main(int argc, char **argv) 
{
    long one[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    long two[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    long three[10] = { 1, 10, 2, 9, 3, 8, 4, 5, 7, 6 };
    long four[11] = { 1, 11, 7, 8, 9, 10, 2, 4, 3, 6, 5};

    long oner[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    long fourr[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };   

    printf("%s\n", "Starting qsort");
    printArr(one, 10);
    qsortr(one, 10, 0, 9);
    testEquals(one, oner, 10);

    printArr(two, 10);
    qsortr(two, 10, 0, 9);
    testEquals(two, oner, 10);
    
    printArr(three, 10);
    qsortr(three, 10, 0, 9);
    testEquals(three, oner, 10);

    printArr(four, 11);
    qsortr(four, 11, 0, 10);
    testEquals(four, fourr, 11);

    printf("%s\n", "Done");
    
    

    return 0;
}
