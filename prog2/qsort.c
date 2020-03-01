/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 02/24/2020                                                      */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: main.c                                                         */
/* PROGRAM PURPOSE:                                                          */
/*    This program forks a child process that executes qsort.c to sort an    */
/*    array using quicksort.  Concurrently, it performs a binary merge on    */
/*    another two arrays by forking a child to execute merge.c.  The arrays  */
/*    are transferred between processes by using shared memory.              */
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
/*    Perform the lomuto partition on the given array with the given start   */
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
/* FUNCTION setArgs                                                          */
/*    Utility sets the argument array before forking and executing           */
/* PARAMETER USAGE:                                                          */
/*    args: the argument array                                               */
/*    left: the left most index                                              */
/*    right: the */
/*void setArgs(char **args, long left, long right, int k, long size)
{
    sprintf(args[0], "%s", "./qsort");
    sprintf(args[1], "%ld", left);
    sprintf(args[2], "%ld", right);
    sprintf(args[3], "%d", k);
    sprintf(args[4], "%ld", size);
    sprintf(args[5], "%c", '\0');
}*/

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

void printArray(long *arr, long start, long end)
{
    long arrLen = countArr(arr, start, end);
    char *msg = "%3s### Q-PROC(%4d): entering with a[%ld..%ld]\n";
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
    /*printf("%s\n", printout);*/
    free(printout);
}



/* 
 * Communication protocol:
 * argv[1] = left
 * argv[2] = right
 * argv[3] = memKey
 * argv[4] = size of memory in bytes
 */
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
    /*char *msg = "%3s Q-PROC(%d): entering with a[%ld..%ld]\n";*/
    int numLen = 0;
    int i = 0;
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
/*    sprintf(buf, msg, "", getpid(), left, right);
    write(1, buf, strlen(buf));*/
 
        
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
    printArray(arr, left, right);
        /*left = 10;
    right = 2;*/
    if (left < right)
    {
        /* Partition the array */
        pivot = lomuto(arr, left, right);
    /*    printArray(arr, left, right);*/
/*        return 0; */
   
        /* Set up the args for the child processes */
        setArgsQsort(argsk1, left, pivot - 1, memID, size);
        setArgsQsort(argsk2, pivot + 1, right, memID, size);

        sprintf(buf, "QZ(%4d): before fork\n", getpid());
        write(1, buf, strlen(buf));
        /* Fork to create the children, then exec to run them */
        if ((pid = fork()) == 0)
        {
            execvp(argsk1[0], argsk1);       
            exit(1);
        }
        sprintf(buf, "QW(%4d): after 1st fork\n", getpid());
        write(1, buf, strlen(buf));          
        if ((pid = fork()) == 0)
        {
            execvp(argsk2[0], argsk2);
            exit(1);
        }
        sprintf(buf, "QP(%4d): Waiting for kids...\n", getpid());
        write(1, buf, strlen(buf));
        
        wait(&pid);
        sprintf(buf, "Kid 1: %s\n", strerror(pid));
        write(1, buf, strlen(buf));
        wait(&pid);   
        sprintf(buf, "Kid 2: %s\n", strerror(pid));
        write(1, buf, strlen(buf));
    }
/*    if ((pid = fork()) < 0)
    {

    } 
    else if (pid == 0)
    {
        if (execvp(argsk1[0], argsk1) < 0)
        {*/
            /* TODO: error handling */
  /*      }
    }
    else
    {
        if ((pid = fork()) < 0)
        {

        } 
        else if (pid == 0)
        {
            if (execvp(argsk2[0], argsk2) < 0)
            {*/
                /*TODO: error handling */
     /*       }
        }   
        else
        {
            wait(&pid);
            wait(&pid); 
        }
        
    }*/
    
   


    
    /* Finally, detach from shared memory */
    pivot = shmdt((void *) arr);
    if (pivot == -1)
    {
        sprintf(buf, "%s\n", "FAILED TO DETACH FROM SHARED MEMORY!!!");
        write(2, buf, strlen(buf));
        exit(1);
    }
       
    
    return 0;
}
