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

/*
void errHandleShm(long *data, int shmID, int eval)
{
    char buf[80];
    if (eval != -1)
        return;
    if (data != NULL)
        shmdt((void *) data);
    sprintf(buf, "*** MAIN: %s\n", "shared mem error...cleaning up + exiting");
    write(2, buf, strlen(buf));
    exit(1);
}*/


/* Creates shared memory and returns its id */
/*int createShm(long **data, struct dataInfo *datai)
{
    int shmID = 0;
    char buf[80];
    key_t key;
    char *msg = "*** MAIN: ";*/

    /* Get the key and print it out */
/*    key = ftok("./", 'j');
    errHandleShm(*data, -1, key);*/

    /* Create the shared memory */
/*    shmID = shmget(key, (datai -> totLen) * sizeof(long), 0666);
    errHandleShm(*data, shmID, shmID);*/

   /* Attach to the shared memory */
/*    *data = (long *) shmat(shmID, NULL, 0);
     errHandleShm(*data, shmID, (int) *data);
     return shmID;
}*/


/* ------------------------------------------------------------------------- */
/* FUNCTION: binSearch                                                       */
/*    Performs a binary search to find and */ 
long binSearch(long i, long *x, long ix, long *y, long iy, char *ms, char c)
{
    char buf[80];
    char *bet = " is found between ";
    char *form = "%s%c[%ld] = %ld%s%c[%ld] = %ld and %c[%ld] = %ld\n";
    char other = 'y';
    long k = 0;
    long j = iy - 1;
    long m = (j - k) / 2 + 1;

    if (c == 'x')
        other = 'y';
    else
        other = 'x';

    while (k < j) 
    {
        if (x[i] < y[m])
        {
            /* Guessed an index that was too large */
            j = m - 1;
            m = (j - k) / 2 + 1; 
        }
        else 
        {
            /* Guessed an index that was too small */
            k = m + 1;
            m = m + (j - k) / 2 + 1;
        }
    }
    
    
    sprintf(buf, form, ms, c, i, x[i], bet, other, m - 1, y[m - 1], 
            other, m, y[m]);
    write(1, buf, strlen(buf));
    return m + i;
}

void merge(long i, long *x, long *y, long *r, long ix, long iy, long ir, char *m, char c) 
{
    long j, k;
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
    
    /*sprintf(buf, "Hello %4d %c[%ld]\n", getpid(), c, i);
    write(1, buf, strlen(buf));
    if (x == NULL)
    {
        sprintf(buf, "X is null%s\n", "");
        write(1, buf, strlen(buf));
    }
    else 
    {
        sprintf(buf, "X: %ld\n", x[0]);
        write(1, buf, strlen(buf));
    }*/
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
        endElem = binSearch(i, x, ix, y, iy, m, c);
    }
    sprintf(buf, oMsg, m, curr, endElem);
    write(1, buf, strlen(buf));
   
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
   /* key_t key;*/
    long *x = NULL;
    long *y = NULL;
    long *res = NULL;   
    long *data = NULL;  
    long startX = 0, startY = 0, startOutput = 0;
    long lenx = 0, leny = 0, leno = 0;
    long dataLen = 0;
    long kiddos = 0;
    long i = 0;
    int pid = 0;

    /*sprintf(buf, format, "", getpid());
    write(1, buf, strlen(buf));*/
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

    startX = atol(argv[1]);
    lenx = atol(argv[2]);
    startY = atol(argv[3]);
    leny = atol(argv[4]);
    startOutput = atol(argv[5]);
    leno = atol(argv[6]);
    shmID = atoi(argv[7]);
    dataLen = atol(argv[8]);
   /* sprintf(buf, "%6s$$$M : %d\n", "", shmID);
    write(1, buf, strlen(buf));*/

    /* get and attach to shared memory with checks */   
    data = (long *) shmat(shmID, NULL, 0);
    /* no need to error check: if I fail to attach, I can't possibly detach */
    if (data == (void *) -1)
        pid = -1;         

    /* Initialize the arrays */
    x = &data[startX];
    sprintf(buf, "buff x: %s\n", "");
    write(1, buf, strlen(buf));
    y = &data[startY];
    res = &data[startOutput];
    sprintf(buf, "X? %ld\n", x[0]);
    write(1, buf, strlen(buf));

    sprintf(buf, "%6s$$$$ M-PROC(%4d): Main merge process\n", "", getpid());
    write(1, buf, strlen(buf)); 

    /* fork m + n kiddos to do the work */
    for (i = 0; i < lenx; i++)
    {
        if ((pid = fork()) < 0)
        {
            sprintf(buf, "%s\n", "Forking error");
            write(2, buf, strlen(buf));
        }
        else if (pid == 0)
        {
            /*strcat(buf, format);
            strcat(buf, "handling x[%ld] = %ld"); 
            sprintf(msgbuf, buf, getpid(), i, x[i]);
*/
        /*    sprintf(msgbuf, "Proc %d handling x[%ld]\n", getpid(), i);
            write(1, msgbuf, strlen(msgbuf)); 
            
            sprintf(buf, "Nice day!%s\n", "");
            write(1, buf, strlen(buf));*/
            sprintf(buf, "%6s$$$ M-PROC(%4d): ", "", getpid());
            merge(i, x, y, res, lenx, leny, leno, buf, 'x');
          /*  sprintf(buf, "merged %ld\n", i);
            write(1, buf, strlen(buf));*/
            exit(0);
        }
        else
        {
            /*sprintf(buf, "Hello %d\n", getpid());
            write(2, buf, strlen(buf));*/
        }
        /* do nothing special for the parent*/
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
            merge(i, y, x, res, leny, lenx, leno, buf, 'y');
            exit(0);
        }
    }
    
    for (i = 0; i < lenx + leny; i++)
        wait(&pid);    
/*{
    sprintf(buf, "Waiting... %s\n", "");
    write(1, buf, strlen(buf));
        wait(&pid);
    sprintf(buf, "Nowait%s\n", "");
    write(1, buf, strlen(buf));
    }*/
    shmdt(x);
    return 0;
}
 


