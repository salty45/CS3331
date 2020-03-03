/* ------------------------------------------------------------------------- */
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





















#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
struct DataInfo
{
    long k;
    long m;
    long n;
    long sk;
    long sm;
    long sn;
    long sdat;
    long datLen;
    long totLen;
};


/* ------------------------------------------------------------------------- */
/* FUNCTION: binSearch                                                       */
/*    Performs a binary search to find and */
long binSearch(long i, long *x, long *y, long iy, char *ms, char c)
{
    char buf[80];
    char *bet = " is found between ";
    char *form = "%s%c[%ld] = %ld%s%c[%ld] = %ld and %c[%ld] = %ld\n";
    char other = 'y';
    long k = 0;
    long j = iy - 1;
    long m = (j + k) / 2;

    if (c == 'x')
        other = 'y';
    else
        other = 'x';

    while (k <= j)
    {
        if (x[i] < y[m])
        {
            /* The value in array x is smaller than the element in y */
            j = m - 1;
            m = (j + k) / 2;
        }
        else
        {
            /* The value at array x[i] is larger than the element at y[m]*/
            k = m + 1;
            m = (j + k) / 2;
        }
    }


    sprintf(buf, form, ms, c, i, x[i], bet, other, m - 1, y[m - 1],
            other, m, y[m]);
    write(1, buf, strlen(buf));
    return k + i;
}

void merge(long i, long *x, long *y, long *r, long iy, char *m, char c)
{
    char buf[80];
    char o = 'y';
    char *elem = "%c[%ld] = %ld";
    char smaller[80];
    char bigger[80];
    char curr[80];
    char *msg = "%s%s is found to be %s than %s\n";
    char *oMsg = "%sabout to write %s into position %ld of the output array\n";
    long endElem = 0;

    /* switch letters if y comes in first */
    if (c == 'y')
        o = 'x';

    /* print out some info */
    sprintf(curr, elem, c, i, x[i]);
    sprintf(buf, "%shandling %s\n", m, curr);
    write(1, buf, strlen(buf));

    if (x[i] < y[0])
    {
        endElem = i;
        sprintf(smaller, elem, o, 0, y[0]);
        sprintf(buf, msg, m, curr, "smaller", smaller);
        write(1, buf, strlen(buf));
    }
    else if (x[i] > y[iy - 1])
    {
        endElem = i + iy;
        sprintf(bigger, elem, o, iy - 1, y[iy - 1]);
        sprintf(buf, msg, m, curr, "larger", bigger);
        write(1, buf, strlen(buf));
    }
    else
    {
        endElem = binSearch(i, x, y, iy, m, c);
    }
    sprintf(buf, oMsg, m, curr, endElem);
    write(1, buf, strlen(buf));
    r[endElem] = x[i];
}


/**
 * Merge.c gets x and y as args as well as a key to shared memory
 *
 * Communication protocol:
 * 1 - starting index of x
 * 2 - size of x
 * 3 - starting index of y
 * 4 - size of y
 * 5 - starting index of data
 * 6 - size of data
 * 7 - shared memory id
 * 8 - total size of shared memory
 */

int main (int argc, char **argv)
{
    char buf[80];
    char *msg = "Incorrect num args:  found";
    char *format = "%6s$$$ M-PROC(%4d): ";
    char msgbuf[80];
    int numArgs = 9;
    int shmID = 0;
    long *x = NULL;
    long *y = NULL;
    long *res = NULL;
    long *data = NULL;
    long startX = 0, startY = 0, startOutput = 0;
    long lenx = 0, leny = 0, leno = 0;
    long dataLen = 0;
    long i = 0;
    int pid = 0;

    /* Confirm the args are all here! */
    if (argc < numArgs || argc > numArgs)
    {
        sprintf(msgbuf, "%s", "");
        strcat(msgbuf, format);
        strcat(msgbuf, "%s %d expected %d\n");
        sprintf(buf, msgbuf, "", getpid(), msg, argc, numArgs);
        write(1, buf, strlen(buf));
        exit(1);
    }

    /* load the arguments */
    startX = atol(argv[1]);
    lenx = atol(argv[2]);
    startY = atol(argv[3]);
    leny = atol(argv[4]);
    startOutput = atol(argv[5]);
    leno = atol(argv[6]);
    shmID = atoi(argv[7]);
    dataLen = atol(argv[8]);

    /* get and attach to shared memory with checks */
    data = (long *) shmat(shmID, NULL, 0);
    /* no need to error check: if I fail to attach, I can't possibly detach */
    if (data == (void *) -1)
        pid = -1;

    /* Initialize the arrays */
    x = &data[startX];
    y = &data[startY];
    res = &data[startOutput];

    sprintf(buf, "%6s$$$ M-PROC(%4d): Main merge process\n", "", getpid());
    write(1, buf, strlen(buf));

    /* fork m + n kiddos to do the work */
    for (i = 0; i < lenx; i++)
    {
        if ((pid = fork()) < 0)
        {
            sprintf(buf, "%6s$$$$ M-PROC(%4d): Forking error\n", "", getpid());
            write(2, buf, strlen(buf));
        }
        else if (pid == 0)
        {
            sprintf(buf, "%6s$$$ M-PROC(%4d): ", "", getpid());
            merge(i, x, y, res, leny, buf, 'x');
            exit(0);
        }
    }

    for (i = 0; i < leny; i++)
    {
        if ((pid = fork()) < 0)
        {
            sprintf(buf, "%s\n", "Forking error");
            write(2, buf, strlen(buf));
        }
        else if (pid == 0)
        {
            sprintf(buf, "%6s$$$ M-PROC(%4d): ", "", getpid());
            merge(i, y, x, res, lenx, buf, 'y');
            exit(0);
        }
    }

    for (i = 0; i < lenx + leny; i++)
        wait(&pid);

    shmdt(x);
    return 0;
}
