/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 03/02/2020                                                      */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: qsort.c                                                        */
/* PROGRAM PURPOSE:                                                          */
/*    This program recursively forks child processes to sort an input array  */
/*    using quicksort with a lomuto partition.  Each process prints out some */
/*    messages about its incoming array, partition, and when it exits.  The  */
/*    array is accessed by attaching to shared memory using a key specified  */
/*    in program arguments.                                                  */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* --------------------------------------------------------------------------*/
/* FUNCTION swap:                                                            */
/*    Swaps the elements at indices a and b in array arr                     */
/* PARAMETER USAGE:                                                          */
/*    arr: the array in which to perform the swap                            */
/*    a: the index of the first number to swap                               */
/*    b: the index of the second number to swap                              */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
void swap(long *arr, long a, long b)
{
    long x = arr[a];
    arr[a] = arr[b];
    arr[b] = x;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION lomuto                                                           */
/*    Perform the Lomuto partition on the given array with the given start   */
/* PARAMETER USAGE:                                                          */
/*    arr: array to partion                                                  */
/*    l: the left most index                                                 */
/*    r: the right most index                                                */
/* FUNCTION CALLED:                                                          */
/*    swap: utility to swap values at indices                                */
/* ------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION: setArgsQsort                                                    */
/*    Utility function fills the argument array to use when executing qsort  */
/* PARAMETER USAGE:                                                          */
/*    qargs: pointer to the argument array                                   */
/*    l: the index of the left end of the array                              */
/*    r: the index of the rightmost element in the array                     */
/*    id: the id of the shared memory segment                                */
/*    len: the total length of the shared memory segment                     */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
void setArgsQsort(char *qargs[5], long l, long r, int id, long len)
{
    int i = 0;
    for (i = 0; i < 5; i++)
        qargs[i] = malloc(sizeof(char) * 80);

    sprintf(qargs[0], "%s", "./qsort");
    sprintf(qargs[1], "%ld", l);
    sprintf(qargs[2], "%ld", r);
    sprintf(qargs[3], "%d", id);
    sprintf(qargs[4], "%ld", len);
    qargs[5] = '\0';
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: countArr                                                        */
/*    Utility function counts and returns the number of ASCII characters     */
/*    needed to print the given segment of the given array.                  */
/* PARAMETER USAGE:                                                          */
/*    arr: pointer to the array                                              */
/*    start: the index of the leftmost element in the array                  */
/*    end: the index of the rightmost element in the array                   */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
long countArr(long *arr, long start, long end)
{
    long i = 0;
    long arrLen = 0;
    char buf[12];
    for (i = start; i <= end; i++)
    {
        sprintf(buf, " %ld ", arr[i]);
        arrLen += strlen(buf);
    }
    return arrLen;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: printArray                                                      */
/*    Utility function prints the given message with the given array segment */
/*    as a single write statement.  The message is assumed to be of the      */
/*    format to take arguments of %s %d %ld %ld                              */
/* PARAMETER USAGE:                                                          */
/*    arr: pointer to the array                                              */
/*    start: the index of the leftmost element in the array                  */
/*    end: the index of the rightmost element in the array                   */
/*    msg: the message to be printed, taking four arguments %s %d %ld %ld    */
/* FUNCTION CALLED:                                                          */
/*    countArr: counts the number of elements in the array                   */
/* ------------------------------------------------------------------------- */
void printArray(long *arr, long start, long end, char * msg)
{
    long arrLen = countArr(arr, start, end);
    char buf[120];
    long length = 0;
    char *printout;
    long i;
    sprintf(buf, msg, "", getpid(), start, end);
    length = arrLen + strlen(buf) + 3 + 7;
    printout = malloc(sizeof(char) * length);
    sprintf(printout, "%s", "");
    strcat(printout, buf);
    sprintf(buf, "%6s", "");
    strcat(printout, buf);
    for (i = start; i <= end; i++)
    {
        sprintf(buf, " %ld ", arr[i]);
        strcat(printout, buf);
    }
    strcat(printout, "\n\n");
    write(1, printout, strlen(printout));
    free(printout);
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: countArr                                                        */
/*    Utility function frees the subarrays of the given array of size len    */
/* PARAMETER USAGE:                                                          */
/*    arr: pointer to the array                                              */
/*    len: the size of the array                                             */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
void freeArgArray(char *arr[], int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
        free(arr[i]);
}

/* ------------------------------------------------------------------------- */
/* FUNCTION main:                                                            */
/*     Main reads in the command-line arguments and parses them according to */
/*     the following protocol:                                               */
/*         argv[1] = left index of the array to sort                         */
/*         argv[2] = right index of the array to sort                        */
/*         argv[3] = shared memory id                                        */
/*         argv[4] = size of shared memory segment in bytes                  */
/*     Main then attaches to shared memory prints out some messages,         */
/*     partitions the array using the Lomuto algorithm, and forks two child  */
/*     processes to run qsort with subsections of the input array.  After    */
/*     After waiting for the children to complete, main prints out the sorted*/
/*     subarray, detaches from shared memory, and exits.                     */
/* PARAMETER USAGE:                                                          */
/*    argc: the number of command line arguments                             */
/*    argv: array of command-line arguments                                  */
/* FUNCTION CALLED:                                                          */
/*    printArray:  utility to print out an array with a message              */
/*    lomuto: performs a Lomuto partition on an array                        */
/*    setArgsQsort: utility creates the arguments to execute qsort           */
/*    freeArgArray: utility function that frees the argument subarrays       */
/* ------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    char buf[80];
    int numArgs = 5;
    long pivot = 0;
    char *argsk1[numArgs + 1];
    char *argsk2[numArgs + 1];
    long left, right;

    int pid;
    int memID;
    long *arr;
    long size;
    char *msg1 = "%3s### Q-PROC(%4d): entering with a[%ld..%ld]\n";
    char *msg2 = "%3s### Q-PROC(%4d): section a[%ld..%ld] sorted\n";
    char *msgPiv = "%3s### Q-PROC(%4d): pivot element is a[%ld] = %ld\n";

    /* Verify args have come in correctly */
    if (argc < numArgs || argc > numArgs)
    {
        sprintf(buf, "%s\n", "Improper num args");
        write(1, buf, strlen(buf));
        exit(1);
    }

    /* Read in the command-line args */
    left = atol(argv[1]);
    right = atol(argv[2]);
    memID = atoi(argv[3]);
    size = atol(argv[4]);


    /* Attach to the shared memory */
    arr = (long *) shmat(memID, NULL, 0);
    if (arr == (void *) -1)
    {
        sprintf(buf, "%s\n", "failed to attach to mem");
        write(2, buf, strlen(buf));
        shmdt((void *) arr);
        exit(1);
    }

    /* Print the array */
    printArray(arr, left, right, msg1);

    /* Partition the array */
    pivot = lomuto(arr, left, right);

    sprintf(buf, msgPiv, "", getpid(), pivot, arr[pivot]);
    write(1, buf, strlen(buf));

    /* Set up the args for the child processes */
    setArgsQsort(argsk1, left, pivot - 1, memID, size);
    setArgsQsort(argsk2, pivot + 1, right, memID, size);

    /* Fork to create the children, then exec to run them */
    if (left <= pivot - 1)
    {
        if ((pid = fork()) == 0)
        {
            execvp(argsk1[0], argsk1);
            exit(1);
        }
    }

    if (pivot + 1 <= right)
    {
        if ((pid = fork()) == 0)
        {
            execvp(argsk2[0], argsk2);
            exit(1);
        }
    }

    wait(&pid);
    wait(&pid);
    printArray(arr, left, right, msg2);


    /* Finally, detach from shared memory */
    pivot = shmdt((void *) arr);
    if (pivot == -1)
    {
        sprintf(buf, "%s\n", "FAILED TO DETACH FROM SHARED MEMORY!!!");
        write(2, buf, strlen(buf));
        exit(1);
    }

    /* Free the argument arrays */
    freeArgArray(argsk1, numArgs + 1);
    freeArgArray(argsk2, numArgs + 1);

    sprintf(buf, "%3s### Q-PROC(%4d): exits%s\n", "", getpid(), "");
    write(1, buf, strlen(buf));

    return 0;
}
