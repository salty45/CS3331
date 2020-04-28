/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                   UserID: selarkin    */
/* DUE DATE: 04/27/2020                                                     */
/* PROGRAM ASSIGNMENT #6                                                    */
/* FILE NAME: thread-main.cpp                                               */
/* PROGRAM PURPOSE:                                                         */
/*      Performs a concurrent multi-threaded sieve to find all the prime    */
/*      numbers less than or equal to a given number in the range [3, 100]. */
/* ------------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>

#include "thread.h"

/* ------------------------------------------------------------------------ */
/* FUNCTION: main                                                           */
/*      Creates the array of primes, loads the number to go up to, creates  */
/*      and starts the master thread.                                       */
/* PARAMETER USAGE:                                                         */
/*      argc: the number of command-line arguments                          */
/*      argv: the command-line argument array, as strings                   */
/* FUNCTION CALLED:                                                         */
/*      Master: constructor for the master thread                           */
/* ------------------------------------------------------------------------ */
int main(int argc, char **argv)
{
    int num;
    int *primes;

    Master *master;

    /* Set the default value of num */
    if (argc < 2)
        num = 30;
    else 
        num = atoi(argv[1]);
    
    primes = (int*) malloc((num / 4 + 4) * sizeof(int));
    master = new Master(primes, num);
    master->Begin();
    master->Join();
    delete(master);
    free(primes);
    return 0;
}
