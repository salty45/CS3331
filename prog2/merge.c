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
}


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
    long *x;
    long *y;
    long *res;   
    long *data;  
    long startX, startY, startOutput;
    long lenx, leny, leno;
    long kiddos;
    long i = 0;
    int pid;

    sprintf(buf, format, "", getpid());
    write(1, buf, strlen(buf));
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

    shmID = atoi(argv[7]);
    sprintf(buf, "%6s$$$M : %d\n", "", shmID);
    write(1, buf, strlen(buf));

    /* get and attach to shared memory with checks */   
    data = (long *) shmat(shmID, NULL, 0);
    /* no need to error check: if I fail to attach, I can't possibly detach */
    if ((int) data == -1)
        pid = -1;          
    sprintf(buf, "%6s$$$$ M-PROC(%4d): attached %d  %s\n", "", getpid(), 
            pid, strerror(errno));
    write(1, buf, strlen(buf)); 
    return 2;
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
            strcat(buf, format);
            strcat(buf, "handling x[%ld] = %ld"); 
            sprintf(buf, buf, getpid(), i, x[i]);
            write(1, buf, strlen(buf)); 
        }
        /* do nothing special for the parent*/
    }
    for (i = 0; i < lenx; i++)
        wait(&pid);
    shmdt(x);
    return 0;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: binSearch                                                       */
/*    Performs a binary search to find and */ 
long binSearch(long i, long *x, long ix, long *y, long iy)
{
    long k = 0;
    long j = iy - 1;
    long m = (j - k) / 2 + 1;
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
    return k + i;
}

void merge(long i, long *x, long *y, long *r, long ix, long iy, long ir) 
{
    long j, k;
    if (x[i] < y[0])
        r[i] = x[i];
    else if (x[i] > y[iy - 1])
        r[i] = x[i+iy];
    else
        r[binSearch(i, x, ix, y, iy)] = x[i];
    exit(0);
}


