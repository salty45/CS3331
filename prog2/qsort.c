#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void swap(long *arr, long a, long b)
{
    long x = arr[a];
    arr[a] = arr[b];
    arr[b] = x;
}


int lomuto(long *arr, long l, long r)
{
    long p = arr[l];
    long si = l;
    long i = 0;
    for (i = l + 1; i <= r; i++)
    {
        if (arr[i] < p)
        {
            si++;
            swap(arr, si, i);
        }
    }
    swap(arr, l, si);
    return si;
}
void setArgs(char **args, long left, long right, int k, long size)
{
    sprintf(args[0], "%s", "./qsort");
    sprintf(args[1], "%ld", left);
    sprintf(args[2], "%ld", right);
    sprintf(args[3], "%d", k);
    sprintf(args[4], "%ld", size);
    sprintf(args[5], "%c", '\0');
}

/* 
 * Communication protocol:
 * argv[1] = left
 * argv[2] = right
 * argv[3] = memKey
 * argv[4] = size of memory in bytes
 */
int main(int argc, char **argv)
{
    char buf[80];
    int numArgs = 5;
    long pivot = 0;
    char argsk1[numArgs + 1][80];
    char argsk2[numArgs + 1][80];
    long left, right;
    int k;     
    
    int pid; 
    key_t key;
    int memID;
    long *arr;
    long size;
    char *msg = "QSORT!";

    sprintf(buf, "%s\n", "Hello QSORT");
    write(2, buf, strlen(buf));
    /* Verify args have come in correctly */
    if (argc < numArgs || argc > numArgs)
    {
        sprintf(buf, "%s\n", "Improper num args");
        write(1, buf, strlen(buf));
        exit(1);
    }

    /* Read in the command-line args */
    left = atol(argv[1]);
    right = atol(argv[2]);
    k = atoi(argv[3]);
    size = atol(argv[4]);    
    sprintf(buf, "Mem id: %d\n", k);
    write(2, buf, strlen(buf));

     /* Get the key and print it out */
    key = ftok("./", 'j');
    sprintf(buf, "%sshared memory key = %d\n", msg, key);
    write(1, buf, strlen(buf));
 /*   errHandleShm(*data, -1, key);*/

    /* Create the shared memory */
    k = shmget(key, sizeof(long) * size, 0666);
    left = errno;
    
   /* errHandleShm(*data, shmID, shmID);*/
    sprintf(buf, "%sshared memory created: %d\n", msg, k);
    write(1, buf, strlen(buf));
   
    if (k == -1)
    {
        sprintf(buf, "%s %s\n", "k = -1", strerror(left));
        write(2, buf, strlen(buf));
        perror(left);
        if (left == EEXIST)
        {
            sprintf(buf, "%s\n", "memissue");
            write(2, buf, strlen(buf));
        }
    }
         
    /* Attach to the shared memory */
    arr = (long *) shmat(k, NULL, 0);
    if ((int) arr == 0)
    {
        sprintf(buf, "%s\n", "mem fail qsort");
        write(1, buf, strlen(buf));
    }
    return 0;
    /* Attach to shared mem */
    /*key = ftok("./", k);
    if (key == -1)
        exit(1);

    memID = shmget(key, sizeof(long) * size, 0666);
    if (memID == -1)
        exit(1);

    arr = (long *) shmat(memID, NULL, 0);*/
    arr = (long *) shmat(k, NULL, 0);
    if ((int) arr < 0)
    {
        sprintf(buf, "%s\n", "failed to attach to mem");
        write(2, buf, strlen(buf));
        exit(1);
    }

    sprintf(buf, "%s\n", "MEM good");
    write(2, buf, strlen(buf)); 
    
    /* Partition the array */
    pivot = lomuto(arr, left, right);
    
    sprintf(buf, "%3s Q-PROC(%d): entering with a[%ld..%ld]\n", "", getpid(),
            left, right);
    write(1, buf, strlen(buf));
    
    /* Set up the args for the child processes */
    setArgs(argsk1, left, pivot - 1, k, size);
    setArgs(argsk2, pivot + 1, right, k, size);

    /* Fork to create the children, then exec to run them */
    if ((pid = fork()) < 0)
    {

    } 
    else if (pid == 0)
    {
        if (execvp(argsk1[0], argsk1) < 0)
        {
            /* TODO: error handling */
        }
    }
    else
    {
        if ((pid = fork()) < 0)
        {

        } 
        else if (pid == 0)
        {
            if (execvp(argsk2[0], argsk2) < 0)
            {
                /*TODO: error handling */
            }
        }   
        else
        {
            wait(&pid);
            wait(&pid); 
        }
        
    }
    
   


    
    /* Finally, detach from shared memory */
    pivot = shmdt((void *) arr);
    if (pivot == -1)
    {
        sprintf(buf, "%s\n", "FAILED TO DETACH FROM SHARED MEMORY!!!");
        write(2, buf, strlen(buf));
        exit(1);
    }
       
    
    return 0;
}
