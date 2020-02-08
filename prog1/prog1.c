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
#include <math.h>
#include <time.h>

double random(double a, double b);
void buffonsProcess(long r);
void bernoulliProcess(long m);
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
    sprintf(buf, "%lf\n", random(0, 10));
    write(1, buf, strlen(buf));
    sprintf(buf, "%lf\n", random(2, 9));
    write(1, buf, strlen(buf));

/*    buffonsProcess(r);*/
    bernoulliProcess(m);
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


double random(double a, double b)
{
/*    time_t randomSeed = time(NULL);
    float randn;
    double randNum;
    srand(randomSeed);*/
    float randn = rand();
    double randNum = randn/RAND_MAX;
    return randNum * (b - a) + a;
}


void buffonsProcess(long r)
{
    char buf[80];
    const double PI = acos(-1.0);
    const long L = 1;
    const long G = 1;
    double d = random(0, 1);
    double a = random(0, 2 * PI);

    double prob = 0;
    double numCrossings = 0;
    long i = 0;
    double throw = 0;

    sprintf(buf, "%9s%s\n", "", "Buffon's Needle Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%9sInput Number %ld", "", r);
    write(1, buf, strlen(buf));

    /*// TODO: fix the indentation issues*/

    for (i = 0; i < r; i++)
    {
        /* Get the random numbers */
        d = random(0, 1);
        a = random(0, 2 * PI);
        throw = d + L * sin(a);
    
        /* Did the throw land on a line? */
        if (throw < 0 || throw > G)
            numCrossings++;   
    }

    prob = numCrossings / r;
      
    sprintf(buf, "%9sEstimated Probablility is %lf\n", "", prob);
    write(1, buf, strlen(buf));    
}

void integrationProcess(long s)
{
    char buf[80];
    const double PI = acos(-1.0);
    double a = random(0, PI);
    double b = random(0, 1);
    double point = 0;
    long i = 0;
    long t = 0;
    double result = 0;


    sprintf(buf, "%12s%s\n", "", "Integration Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%12s%s %ld\n", "", "Input Number", s);
    write(1, buf, strlen(buf));
    

    for (i = 0; i < s; i++)
    {
        a = random(0, PI);
        b = random(0, 1);
 
        if (b <= sin(a))
            t++;       

    }

    result = ((double)t)/s;   

    sprintf(buf, "%12s%s %ld\n", "", "Total Hit", t);
    write(1, buf, strlen(buf));
    sprintf(buf, "%12s%s %lf\n", "", "Estimated Area is", result);
    write(1, buf, strlen(buf));
    sprintf(buf, "%12s%s\n", "", "Integration Process Exits");
    write(1, buf, strlen(buf));    
}

void bernoulliProcess(long m)
{
    char buf[80];
    unsigned long i = 0;
    double e = 0;
    double diff = 0;
    double num = 0;
    const double E = exp(1.0);

    sprintf(buf, "%3s%s\n", "", "Approxmiation of e Process Started");
    write(1, buf, strlen(buf));
    sprintf(buf, "%3s%s %ld\n", "", "Maximum of the Exponent", m);
    write(1, buf, strlen(buf));
    
    for (i = 1; i <=10; i++)
    {
        num = 1 + 1.0/i;
        e = pow(num, i);
        diff = e - E;
        sprintf(buf, "%2s%18lu %18f %18f\n", " ", i, e, diff);
        write(1, buf, strlen(buf));
    }       

    sprintf(buf, "%s\n", "After 1st loop");
    write(1, buf, strlen(buf));
    for (i = 16; i < m; i *= 2)
    {
        num = 1 + 1.0/i;
        e = pow(num, i);
        diff = e - E;
        sprintf(buf, "%3s%18lu %18f %18f\n", "", i, e, diff);
        write(1, buf, strlen(buf));
    }

    sprintf(buf, "%3s%s\n", "", "Approximation of e Process Exits");
    write(1, buf, strlen(buf));
}
