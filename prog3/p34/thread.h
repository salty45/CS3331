/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 03/18/2020                                                      */
/* PROGRAM ASSIGNMENT 3                                                      */
/* FILE NAME: thread.h                                                       */
/*            Head file for class EOSortThread definition                    */
/* PROGRAM PURPOSE :                                                         */
/*    A program to perform a multithreaded even-odd sort on an array.        */
/* ------------------------------------------------------------------------- */

#include "ThreadClass.h"

class EOSortThread : public Thread
{
    public:
        EOSortThread(long one, long two, bool *f, int **arr, int **flags, int *turn, int length);
    private:
        void ThreadFunc();  /* thread body */
        long a;             /* First index */
        long b;             /* Second index */
        int *x;             /* Array */
        bool *flag;         /* Whether there have been swaps this iteration */
        int *fl;
        int *tur;
        int len;
};
