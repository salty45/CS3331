/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                   UserID: selarkin    */
/* DUE DATE: 04/27/2020                                                     */
/* PROGRAM ASSIGNMENT #6                                                    */
/* FILE NAME: thread.h                                                      */
/* PROGRAM PURPOSE:                                                         */
/*      Performs a concurrent multi-threaded sieve to find all the prime    */
/*      numbers less than or equal to a given number in the range [3, 100]. */
/*      This file contains the definitions for the master and prime threads.*/
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include "ThreadClass.h"

class Sieve: public Thread {
    public:
        Sieve(int *arr, int id, int prev, int i, int *len);       
        SynOneToOneChannel *in; 
        ~Sieve();
    private:
        void ThreadFunc();
        void PrintOut(int i, int k = 0);
        int *primes;
        int *length;
        int index;
        int id;
        Sieve *next;
};

class Master: public Thread {
    public:
        Master(int *arr, int num);
        ~Master();
    private:
        void ThreadFunc();
        void PrintOut(int i, int k = 0);
        int *primes;
        int n;
        Sieve *p2;
        int length;
};
