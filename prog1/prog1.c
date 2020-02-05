/* --------------------------------------------------------------------------*/
/* NAME : Sarah Larkin                                     User ID: selarkin */
/* DUE DATE: 02/07/2020                                                      */
/* PROGRAM ASSIGNMENT 1                                                      */
/* FILE NAME : prog1.c                                                       */
/* PROGRAM PURPOSE:                                                          */
/*    This program runs four non-trivial processes concurrently plus a parent
 *    process.
 *    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <limits.h>

int main(int argc, char **argv)
{
    char buf[80];
    long m, r, s;
    int n;    

    /* Check for command-line argument correctness */
    if (argc < 5) 
    {
        sprintf(buf, "%s\n", "Program has too few arguments");
        write(1, buf, strlen(buf));
        exit(1);
    } 
    else if (argc > 5)
    {
        sprintf(buf, "%s\n", "Program has too many arguments");
        write(1, buf, strlen(buf));
        exit(1);
    }

    /* Store the arguments into their appropriate variables */
    m = atol(argv[1]);
    n = atoi(argv[2]);
    r = atol(argv[3]);
    s = atol(argv[4]);

    /* Print out some stuff */
    sprintf(buf, "%s\n", "Main Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "Fibonacci Number %d\n", n);
    write(1, buf, strlen(buf));
    sprintf(buf, "Buffon's Needle Iterations = %lu\n", r);
    write(1, buf, strlen(buf));
    sprintf(buf, "%-26s = %lu\n", "Integration Iterations", s);
    write(1, buf, strlen(buf));
    sprintf(buf, "%-26s = %lu\n", "Approx. e Iterations", m);
    write(1, buf, strlen(buf));

    /* Then do the forking for processes */

    /* Finally print out and exit */
    sprintf(buf, "%s\n", "Main Process Exits");
    write(1, buf, strlen(buf));
    exit(0);   
    
}

long fibRecurse(int n) 
{
    if (n <= 2)
        return 1;
    
    return fibRecurse(n - 1) + fibRecurse(n - 2);
}

void fibonacciProcess(int n)
{
    char buf[80];
    sprintf(buf, "%6s%s\n", "", "Fibonacci Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%6sInput Number %d\n", "", 10);
    write(1, buf, strlen(buf));
    long fibonacci = fibRecurse(n);
    sprintf(buf, "%6sFibonacci Number f(%d) is %ld\n", "", n, fibonacci);
    write(1, buf, strlen(buf));
    sprintf(buf, "%6s%s\n", "", "Fibonacci Process Exits");
    write(1, buf, strlen(buf));
    exit(0);
}

void buffonsProcess(long r)
{
    char buf[80];
    long l = 1;
    long g = 1;
    double d = random(0, 1)
    double a = random(0, 2 * PI)

    sprintf(buf, "%9s%s\n", "", "Buffon's Needle Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%9sInput Number %ld", "", r);
    write(1, buf, strlen(buf));
    // TODO: fix the indentation issues
    sprintf(buf, "%9sEstimated Probablility is %lf\n", "", prob);
    write(1, buf, strlen(buf));    
}
