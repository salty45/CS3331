/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 03/02/2020                                                      */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: merge.c                                                        */
/* PROGRAM PURPOSE:                                                          */
/*    This program merges two input arrays into a single sorted output array */
/*    by forking multiple children to execute a modified binary search to    */
/*    find each element's position in the new output array.  All three arrays*/
/*    are accessed through attaching to a shared memory segment.             */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION: binSearch                                                       */
/*    Performs a modified binary search to find and return the index of an   */
/*    element in a new array based on its location in its current array and  */
/*    its value relative to a comparison array.                              */
/* PARAMETERS:                                                               */
/*    i: the index in the current array                                      */
/*    x: pointer to the current array                                        */
/*    y: pointer to the comparison array                                     */
/*   iy: length of the comparison array                                      */
/*   sm: message to print regarding the arrays - used for formatted printing */
/*    c: the name of the current array as a char                             */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
long binSearch(long i, long *x, long *y, long iy, char *ms, char c)
{
    char buf[80];
    char *bet = " is found between ";
    char *form = "%s%c[%ld] = %ld%s%c[%ld] = %ld and %c[%ld] = %ld\n";
    char other = 'y';
    long k = 0;
    long j = iy - 1;
    long m = (j + k) / 2;

    /* Set the characters to be printed out */
    if (c == 'x')
        other = 'y';
    else
        other = 'x';

    /* perform the binary search */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION: merge                                                           */
/*     Performs a merge to create a new sorted array by finding a new index  */
/*     for element x[i] in the output array (based on its current position   */
/*     and its value relative to the elements of a comparison array) and     */
/*    storing it there.                                                      */
/* PARAMETERS:                                                               */
/*     i: the current index of the element                                   */
/*     x: pointer to the current array                                       */
/*    y: pointer to the comparison array                                     */
/*    r: pointer to the new array                                            */
/*   iy: length of the comparison array                                      */
/*    m: message to print regarding the arrays - used for formatted printing */
/*    c: the name of the current array as a char                             */
/* FUNCTION CALLED:                                                          */
/*    binSearch: perform a modified binary search to find the index in the   */
/*    new output array                                                       */
/* ------------------------------------------------------------------------- */
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

    /* Is x[i] within the bounds of the comparison array? */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION main:                                                            */
/*     Main reads in the command-line arguments and parses them according to */
/*     the following communication protocol:                                 */
/*         argv[1]: the starting index of array x                            */
/*         argv[2]: the size of array x                                      */
/*         argv[3]: the starting index of array y                            */
/*         argv[4]: the size of array y                                      */
/*         argv[5]: the starting index of the output array                   */
/*         argv[6]: the size fo the output array                             */
/*         argv[7]: the shared memory id                                     */
/*         argv[8]: the total size in bytes of the shared memory segment     */
/*     Main then attaches to shared memory, prints out some information, and */
/*     forks one child per element in x and y to find its location in the    */
/*     output array.  Main then waits for all its children, detaches from    */
/*     shared memory, and exits.                                             */
/* PARAMETER USAGE:                                                          */
/*    argc: the number of command line arguments                             */
/*    argv: array of command-line arguments                                  */
/* FUNCTION CALLED:                                                          */
/*    merge: each child calls merge to find its place in the new array       */
/* ------------------------------------------------------------------------- */
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

    /* fork m + n child processes to do the work in the next two loops*/
    /* fork m child processes */
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

    /* fork n child processes */
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

    /* wait for children */
    for (i = 0; i < lenx + leny; i++)
        wait(&pid);

    shmdt(x);
    return 0;
}
