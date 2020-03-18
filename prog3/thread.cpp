/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 03/18/2020                                                      */
/* PROGRAM ASSIGNMENT 3                                                      */
/* FILE NAME: thread.cpp                                                     */
/*     Implementation of EOSort thread to compare two adjacent elements in   */
/*     an array and swap them if needed.                                     */
/* PROGRAM PURPOSE :                                                         */
/*    A program to perform a multithreaded even-odd sort on an array.        */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* FUNCTION EOSort Class:                                                    */
/*     Constructor for the EOSort class initializes instance variables and   */
/*     prints out a message upon thread creation.                            */
/* PARAMETER USAGE:                                                          */
/*     one:  the index to the first element                                  */
/*     two:  the index to the second element                                 */
/*     f:    whether there has been a swap this iteration                    */
/*     arr:  the array to be sorted                                          */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/* ------------------------------------------------------------------------- */
EOSortThread :: EOSortThread(long one, long two, bool *f, int **arr)
{
    char buf[80];
    a = one;
    b = two;
    x = *arr;
    flag = f;
    sprintf(buf, "%8sThread %ld Created\n", "", a);
    write(1, buf, strlen(buf));
}

/* ------------------------------------------------------------------------- */
/*  FUNCTION ThreadFunc:                                                     */
/*      EOSort Thread Class body: compare two adjacent elements in the array */
/*      and swap them if they are out of order.                              */
/*  PARAMETER USAGE:                                                         */
/*      none                                                                 */
/*  FUNCTION CALLED:                                                         */
/*      none                                                                 */
/* ------------------------------------------------------------------------- */
void EOSortThread::ThreadFunc()
{
    int tmp = 0;
    char buf[80];

    /* Call the parent thread function */
    Thread::ThreadFunc();

    /* print out some info */
    sprintf(buf, "%8sThread %ld compares x[%ld] = %d and x[%ld] = %d\n", 
            "", a, b, x[b], a, x[a]);
    write(1, buf, strlen(buf));

    /* swap the elements if they are out of order */
    if (x[a] < x[b])
    {
        tmp = x[a];
        x[a] = x[b];
        x[b] = tmp;
        *flag = true;
        sprintf(buf, "%8sThread %ld swaps x[%ld] and x[%ld]\n", "", a, b, a);
        write(1, buf, strlen(buf));
    }
    
    /* print out and exit via call to thread library */
    sprintf(buf, "%8sThread %ld exits\n", "", a);
    write(1, buf, strlen(buf));   
    Exit();
}

