/* ------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
/**
 * Merge.c gets x and y as args as well as a key to shared memory
 *
 * Communication protocol:
 * 1 - size of x
 * 2 - size of y
 * note: size of returned array will be size(x) + size(y) + 1
 * 3 - key x
 * 4 - key y
 * 5 - key output
 * 
 */

int main (int argc, char **argv)
{
    char buf[80];
    char *msg = "Incorrect num args:  found";

    int shmIDx;
    int shmIDy;
    int shmIDres;
    key_t key;
    long *x;
    long *y;
    long *res;   

    /* Confirm the args are all here! */
    if (argc < 6 || argc > 6)
    {
        sprintf(buf, "%6s$$$ M-PROC(%4d) %s %d expected 6\n", "", getpid(), 
                msg, argc);
        write(1, buf, strlen(buf));
        exit(0);
    }

    /* get and attach to shared memory with checks */   
    key = ftok("./", argv[3]);
    if (key == -1)
        exit(2);
    shmIDx = shmget(key, sizeof(long) * atol(argv[1]), 0666);
    
    /* check for error - exiting 2 for shared mem not grabbed */
    if (shmIDx == -1)
        exit(2);
    x = (long *) shmat(shmIDx, NULL, 0);
    if ((int) x < 0)
    {
        /* TODO: clean up and exit */
        shmdt(x);
        exit(2);
    }

    /* fork m + n kiddos to do the work */
     
    shmdt(x);
    return 0;
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



