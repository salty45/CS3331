/* --------------------------------------------------------------------------*/
/* NAME : Sarah Larkin                                     User ID: selarkin */
/* DUE DATE: 02/12/2020                                                      */
/* PROGRAM ASSIGNMENT 1                                                      */
/* FILE NAME : prog1.c                                                       */
/* PROGRAM PURPOSE:                                                          */
/*    This program runs a parent and four non-trivial processes concurrently */
/*    to compute the nth Fibonacci number, solve the Buffon's Needle problem,*/
/*    calculate the area under the curve sin(x), and estimate Euler's number */
/*    using Bernoulli's equation per the assignment specifications.          */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <limits.h>
#include <math.h>
#include <time.h>

/* ------------------------------------------------------------------------- */
/* FUNCTION: fibRecurse                                                      */
/*     Recursively calculates and returns the nth Fibonacci number.          */
/* PARAMETER USAGE:                                                          */
/*     n: the index of the Fibonacci number to calculate                     */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/* ------------------------------------------------------------------------- */
long fibRecurse(int n) /* TODO: do I need to check for invalid input? */
{
    /* Base case */
    if (n <= 2)
        return 1;

    /* Recursion */
    return fibRecurse(n - 1) + fibRecurse(n - 2);
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: fibonacciProcess                                                */
/*     Calculates and prints the nth Fibonacci number                        */
/* PARAMETER USAGE:                                                          */
/*     n: the index of the Fibonacci number to calculate                     */
/* FUNCTION CALLED:                                                          */
/*     fibRecurse(n): utility function recursively calculates nth fibonacci  */
/*                    number                                                 */
/* ------------------------------------------------------------------------- */
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
    /*exit(0);*//* TODO: exit vals */
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: drandom                                                         */
/*     Calculates and returns a random number in the range [a, b).           */
/* PARAMETER USAGE:                                                          */
/*     a: the starting point for the random range                            */
/*     b: the ending point for the random range                              */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/* ------------------------------------------------------------------------- */
double drandom(double a, double b)
{
    double randNum = ((float)rand())/RAND_MAX;
    return randNum * (b - a) + a;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION buffonsProcess:                                                  */
/*     This function performs the Buffon's Needle process to determine the   */
/*     number of times a needle thrown on the floor crosses a dividing line. */
/* PARAMETER USAGE:                                                          */
/*    r:  the number of random throws                                        */
/* FUNCTION CALLED:                                                          */
/*    drandom:  utility function to generate random numbers in a range       */
/* ------------------------------------------------------------------------- */
void buffonsProcess(long r)
{
    char buf[80];
    const double PI = acos(-1.0);
    const long L = 1;
    const long G = 1;
    double d = drandom(0, 1);
    double a = drandom(0, 2 * PI);

    double prob = 0;
    double numCrossings = 0;
    long i = 0;
    double throw = 0;

    /* Print out some information */
    sprintf(buf, "%9s%s\n", "", "Buffon's Needle Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%9sInput Number %ld\n", "", r);
    write(1, buf, strlen(buf));

    /* Perform r random throws */
    for (i = 0; i < r; i++)
    {
        /* Get the random numbers */
        d = drandom(0, 1);
        a = drandom(0, 2 * PI);
        throw = d + L * sin(a);

        /* Did the throw land on a line? */
        if (throw < 0 || throw > G)
            numCrossings++;
    }

    /* Calculate the probability and print out the results */
    prob = numCrossings / r;

    sprintf(buf, "%9sEstimated Probablility is %.5f\n", "", prob);
    write(1, buf, strlen(buf));
    sprintf(buf, "%9s%s\n", "", "Buffon's Needle Process Exits");
    write(1, buf, strlen(buf));
}

/* ------------------------------------------------------------------------- */
/* FUNCTION integrationProcess:                                              */
/*     This function performs the integration process to find the area under */
/*     the curve sin(x) as described in the assignment.                      */
/* PARAMETER USAGE:                                                          */
/*    s:  the number of points to randomly pick in the rectangle PI x 1      */
/* FUNCTION CALLED:                                                          */
/*    drandom:  utility function to generate random numbers in a range       */
/* ------------------------------------------------------------------------- */
void integrationProcess(long s)
{
    char buf[80];
    const double PI = acos(-1.0);
    double a = drandom(0, PI);
    double b = drandom(0, 1);   
    long i = 0;
    long t = 0;
    double result = 0;

    /* Print out some messages */
    sprintf(buf, "%12s%s\n", "", "Integration Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%12s%s %ld\n", "", "Input Number", s);
    write(1, buf, strlen(buf));

    /* Loop s times picking a random point */
    for (i = 0; i < s; i++)
    {
        a = drandom(0, PI);
        b = drandom(0, 1);

        /* increment if the point is under the curve */
        if (b <= sin(a))
            t++;
    }

    result = ((double)t)/s * PI;

    /* Print some more messages and exit */
    sprintf(buf, "%12s%s %ld\n", "", "Total Hit", t);
    write(1, buf, strlen(buf));
    sprintf(buf, "%12s%s %f\n", "", "Estimated Area is", result);
    write(1, buf, strlen(buf));
    sprintf(buf, "%12s%s\n", "", "Integration Process Exits");
    write(1, buf, strlen(buf));
}

/* ------------------------------------------------------------------------- */
/* FUNCTION bernoulli:                                                       */
/*     Computes Euler's number using the given i for Bernoulli's equation    */
/*     and prints the result as formatted output containing the value of i,  */
/*     the estimation of e, and the difference between the estimation of e   */
/*     and the true value of e.                                              */
/* PARAMETER USAGE:                                                          */
/*     i: the value to be used in Bernoulli's equation                       */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/* ------------------------------------------------------------------------- */ 
void bernoulli(long i)
{
    double num, e, diff;
    char buf[80];
    const double E = exp(1.0);

    /* Perform the computations */
    num = 1 + 1.0/i;
    e = pow(num, i);
    diff = e - E;

    /* Ensure the difference is positive */
    if (diff < 0)
        diff = diff * -1;

    /* Print out the value of i, the estimation of e, and the difference between
     * the estimation and the true value of e. */
    sprintf(buf, "%3s%18lu %22.15f %22.15f\n", "", i, e, diff);
    write(1, buf, strlen(buf));
}

/* ------------------------------------------------------------------------- */
/* FUNCTION bernoulliProcess:                                                */
/*     Uses Bernoulli's equation to compute the value of Euler's number      */
/* PARAMETER USAGE:                                                          */
/*     m:  the maximum value of n in Bernoulli's equation.                   */
/* FUNCTION CALLED:                                                          */
/*     bernoulli(i): helper function computes Bernoulli's equation for the   */
/*                    given i and prints a formatted output per assignment   */
/* ------------------------------------------------------------------------- */
void bernoulliProcess(long m)
{
    char buf[80];
    unsigned long i = 0;

    /* Print out some information */
    sprintf(buf, "%3s%s\n", "", "Approxmiation of e Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%3s%s %ld\n", "", "Maximum of the Exponent", m);
    write(1, buf, strlen(buf));

    /* Loop from 1 to 10 printing the result of computing the formula */
    for (i = 1; i <=10; i++)
        bernoulli(i);

    /* Loop from 16 to m, printing the computation result.  The value of i
     * doubles each iteration. */
    for (i = 16; i < m; i *= 2)
        bernoulli(i);

    /* Print out the exit message */
    sprintf(buf, "%3s%s\n", "", "Approximation of e Process Exits");
    write(1, buf, strlen(buf));
}

/* ------------------------------------------------------------------------- */
/* FUNCTION main:                                                            */
/*     This function reads in four command-line arguments and spawns four    */
/*     child processes to perform four computations concurrently.  It then   */
/*     waits for its children to finish before terminating.                  */
/* PARAMETER USAGE:                                                          */
/*    argc:  the number of command-line arguments                            */
/*    argv:  an array of command-line arguments stored as char arrays        */
/* FUNCTION CALLED:                                                          */
/*    fibonacciProcess(n): recursively computes the nth fibonacci number     */
/*    buffonsProcess(r):  solve the Buffon's Needle problem with r throws    */
/*    integrationProcess(s):  find the area under the curve sin(x)           */
/*    bernoulliProcess(m): compute the value of e                            */
/* ------------------------------------------------------------------------- */
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
    else if (argc > 6) /* TODO: change back to 5!!!! */
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

/* TODO: fork the procs */
    switch(atoi(argv[5])) {
        case 1:
            fibonacciProcess(n);
            break;
        case 2:
            buffonsProcess(r);
            break;
        case 3:
            integrationProcess(s);
            break;
        case 4:
            bernoulliProcess(m);
            break;
    }

    /* Finally print out and exit */
    sprintf(buf, "%s\n", "Main Process Exits");
    write(1, buf, strlen(buf));
    exit(0);

}
