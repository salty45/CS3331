/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                   UserID: selarkin    */
/* DUE DATE: 04/27/2020                                                     */
/* PROGRAM ASSIGNMENT #6                                                    */
/* FILE NAME: thread.cpp                                                    */
/* PROGRAM PURPOSE:                                                         */
/*      Performs a concurrent multi-threaded sieve to find all the prime    */
/*      numbers less than or equal to a given number in the range [3, 100]. */
/*      This file contains the implementation for each thread to receive a  */
/*      number, check if it is a prime, and create a new thread to pass the */
/*      next prime number to.                                               */
/* ------------------------------------------------------------------------ */

#include "thread.h"
#include <string.h>

/* ------------------------------------------------------------------------ */
/* FUNCTION: Master                                                         */
/*      Constructor for the master thread class.  Creates the first prime   */
/*      thread.                                                             */
/* PARAMETER USAGE:                                                         */
/*      arr: pointer to the array of primes                                 */
/*      num: the number to go up to inclusively searching for primes        */
/* FUNCTION CALLED:                                                         */
/*      Sieve: constructor for the prime threads                            */
/* ------------------------------------------------------------------------ */
Master::Master (int *arr, int num) 
{
    primes = arr;
    n = num;
    length = 0;
    UserDefinedThreadID = 1;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: Sieve                                                          */
/*      Constructor for the prime thread class.  Creates a channel between  */
/*      this thread and the thread that created it.  Stores the given prime */
/*      number to the next open index of the primes array.                  */
/* PARAMETER USAGE:                                                         */
/*      arr: pointer to the array of primes                                 */
/*      id: the thread's assigned prime number                              */
/*      prev: the thread id for the parent thread                           */
/*      i: this thread's thread id                                          */
/*      len: pointer to the length of the primes array                      */
/* FUNCTION CALLED:                                                         */
/*      PrintOut: utility function for printed output                       */
/* ------------------------------------------------------------------------ */
Sieve::Sieve(int *arr, int id, int prev, int i, int *len) 
{
    primes = arr;
    UserDefinedThreadID = id;
    index = i;
    this->id = id;
    length = len;
    (*len)++;
    Sieve *next = NULL;
    in = new SynOneToOneChannel("channel", prev, id);  
    primes[index] = id;
    PrintOut(1); 
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ~Sieve                                                         */
/*      Destructor for sieve class                                          */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
Sieve::~Sieve()
{
    if (next != NULL)
        delete(next);
    delete(in);
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ~Master                                                        */
/*      Destructor for master class                                         */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
Master::~Master()
{
    delete(p2);
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ThreadFunc                                                     */
/*      Receives a series of numbers ending in -1 from its parent. Evaluate */
/*      each number to determine whether it is prime, sending it onward and */
/*      creating a child to remember it if necessary.  Finish by sending -1 */
/*      to the child thread if any exists.                                  */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED:                                                         */
/*      PrintOut: utility for printing messages                             */
/* ------------------------------------------------------------------------ */
void Sieve::ThreadFunc()
{
    Thread::ThreadFunc();

    /* Receive numbers from parent */
    int k = 0;
    while (k != -1)
    {   
        in->Receive(&k, sizeof(int));
        
        /* Print message that number is received */
        if (k == -1)
            PrintOut(6);
        else
            PrintOut(2, k);

        /* Decide whether the received number is prime */
        if (k == -1) {
            /*  end was received */
            if (next != NULL) {
                /* Send end to child */
                PrintOut(7);
                next->in->Send(&k, sizeof(int));
                next->Join();
                //delete(next); 
            }
            Exit();
        } else if (k % id == 0) {
            /* Number received is multiple of memorized number */
            PrintOut(4, k);
        } else if (next == NULL) {
            /* Number received is prime: create a new thread to remember it */
            PrintOut(3, k);
            next = new Sieve(primes, k, UserDefinedThreadID, index+1, length);
            next->Begin();
        } else {
            /* Number received is prime - pass it to child */
            PrintOut(5, k);
            next->in->Send(&k, sizeof(int));
        }
    } 
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ThreadFunc                                                     */
/*      Starts the first prime thread and then sends it a series of numbers */
/*      from 3 up to and including n.  Then sends -1 as the end and joins   */
/*      on the first prime thread before printing out the primes array.     */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED:                                                         */
/*      PrintOut: utility for printing messages                             */
/* ------------------------------------------------------------------------ */
void Master::ThreadFunc() 
{
    Thread::ThreadFunc();
    int i = 0;
    PrintOut(1);

    /* Create and start the first prime thread */ 
    p2 = new Sieve(primes, 2, 1, 0, &length);
    p2->Begin();

    /* Send a series of numbers */
    for (i = 3; i <= n; i++) {
        PrintOut(2, i);    
        p2->in->Send(&i, sizeof(int));  
    }

    /* Send -1 for END */
    i = -1;
    PrintOut(3);
    p2->in->Send(&i, sizeof(int));

    /* Wait for first prime thread to finish and then print out primes array */
    p2 -> Join();
    PrintOut(4);
    Exit();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: PrintOut                                                       */
/*      Utility function for printing messages for prime threads based on   */
/*      input number.                                                       */
/* PARAMETER USAGE:                                                         */
/*      i: the input number indicates which message to print                */
/*      k: indicates an integer value to be printed with some messages.     */
/*          Defaults to 0 if k is not provided.                             */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void Sieve::PrintOut(int i, int k)
{
    char buf[80];
    char space[80];
    int j = 0;

    bzero(buf, 80);

    /* Add in the indent */
    for (j = 0; j <= index; j++)
    {
        sprintf(space, "%2s", "  ");
        strcat(buf, space);
    }

    /* Select the message */
    if (i == 1)
        sprintf(space, "P%d starts and memorizes %d\n", id, id);
    else if (i == 2)
        sprintf(space, "P%d receives %d\n", id, k);
    else if (i == 3)
        sprintf(space, "P%d creates P%d\n", id, k);
    else if (i == 4)
        sprintf(space, "P%d ignores %d\n", id, k);
    else if (i == 5)
        sprintf(space, "P%d sends %d to P%d\n", id, k, next->id);  
    else if (i == 6)
        sprintf(space, "P%d receives END\n", id);
    else if (i == 7)
        sprintf(space, "P%d sends END\n", id);

    /* Append the message and print it */
    strcat(buf, space);
    write(1, buf, strlen(buf));
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: PrintOut                                                       */
/*      Utility function for printing messages for the master threads based */
/*      on input number.                                                    */
/* PARAMETER USAGE:                                                         */
/*      i: the input number indicates which message to print                */
/*      k: indicates an integer value to be printed with some messages.     */
/*          Defaults to 0 if k is not provided.                             */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void Master::PrintOut(int i, int k) 
{
    char buf[80];

    /* Select the message and print it */
    if (i == 4) {
        /* last messages can all be printed sequentially as one option */
        sprintf(buf, "Master prints the complete result:\n ");
        write(1, buf, strlen(buf));

        /* Print the primes array */
        for (i = 0; i < length; i++)
            printf(" %d", primes[i]);
        printf("%s\n", "");
        printf("%s\n", "Master terminates");
    } else {
        /* Other messages to be selected */
        if (i == 1) 
            sprintf(buf, "%s\n", "Master starts");
        else if (i == 2) 
            sprintf(buf, "Master sends %d to P2\n", k);
        else if (i == 3) 
            sprintf(buf, "%s\n", "Master sends END");
        write(1, buf, strlen(buf));
    }
}
