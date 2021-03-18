/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 03/18/2020                                                      */
/* PROGRAM ASSIGNMENT 3                                                      */
/* FILE NAME: thread-main.cpp                                                */
/*     Main program to run the even-odd sort by creating n/2 EOSortThreads   */
/*     up to n times or until no swaps occur in a given iteration            */
/* PROGRAM PURPOSE :                                                         */
/*    A program to perform a multithreaded even-odd sort on an array.        */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "thread.h"

/* ------------------------------------------------------------------------- */
/* FUNCTION printArr:                                                        */
/*    Utility function to print out an array in a specific format            */
/* PARAMETER USAGE:                                                          */
/*    arr:      the array to be printed                                      */
/*    arrLen:   the length of the array to be printed                        */
/*    iter:     the current loop iteration, printed as part of message       */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */ 
void printArr(int *arr, long arrLen, long iter)
{
    int i = 0;
    int j = 0;
    
    /* print the result with a message if the iteration number is positive */
    if (iter > 0)
        printf("Result after iteration %ld:\n", iter);   

    /* print each element in the array */
    for (i = 0; i < arrLen; i++, j++)
    {
        printf("%4d", arr[i]);
     
        /* add a new line after every 20 numbers */   
        if (j == 20)
        {
            j = 0;
            printf("%s\n", "");
        }    
    }
    printf("%s\n\n\n", "");   
}

/* ------------------------------------------------------------------------- */
/* FUNCTION main:                                                            */
/*    Main function reads the array from stdin and creates a loop to run the */
/*    even-odd sort.                                                         */
/* PARAMETER USAGE:                                                          */
/*    argc:     the number of command-line arguments                         */
/*    argv:     the command-line arguments as an array of strings            */
/* FUNCTION CALLED:                                                          */
/*    printArray:  utility to print the array in a formatted manner          */
/*    EOSortThread: constructor for the EOSortThread class                   */
/* ------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    long arrLen;
    int *arr;
    long i = 0;
    long j = 0;
    long k = 0;
    bool flag = false;

    printf("%s\n", "Concurrent Even-Odd Sort");    

    /* Get the length of the array */
    scanf("%ld", &arrLen);
    printf("Number of input data = %ld\n", arrLen);

    /* Allocate space for the array */
    arr = (int *)malloc(sizeof(int) * arrLen);
        
    /* read the array in */
    for (i = 0; i < arrLen; i++)
        scanf("%d", &arr[i]);

    printf("%s\n", "Input array:");
    printArr(arr, arrLen, -1);

    EOSortThread * threads[arrLen/2];
    i = 0;

    /* perform the even-odd sort up to n times, where n is the array length */
    while (i < arrLen)
    {
        printf("Iteration %ld\n", i + 1);
        flag = false; 
     
        /* Even pass */
        printf("%4sEven Pass:\n", "");
        
        /* Create and start n/2 threads */
        for (j = 1, k = 0; j < arrLen; j += 2, k++)
        {
            threads[k] = new EOSortThread(j, j - 1, &flag, &arr);
            threads[k] -> Begin();
        }
        
        /* Wait for and delete n/2 threads */
        for (j = 1, k = 0; j < arrLen; j += 2, k++)
        {
            threads[k] -> Join();
            delete(threads[k]);
        }
      
        /* Odd pass */   
        printf("%4sOdd Pass:\n", "");

        /* Create and start n/2 threads */
        for (j = 2, k = 0; j < arrLen; j += 2, k++)
        {
            threads[k] = new EOSortThread(j, j - 1, &flag, &arr);
            threads[k] -> Begin();
        }

        /* Wait for and delete n/2 threads */
        for (j = 2, k = 0; j < arrLen; j += 2, k++)
        {
            threads[k] -> Join();
            delete(threads[k]);
        }
     
        printArr(arr, arrLen, i + 1);   
        
        /* Perform check for early breakout */
        if (flag == false)
            break;
        i++;
    }

    printArr(arr, arrLen, i + 1);
    free(arr);


    printf("Done %s\n", "");

    return 0;
}
