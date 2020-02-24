#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct dataInfo 
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

long initArray(long **arr)
{
    long a = 0, i = 0;
    scanf("%ld", &a);
    *arr = malloc(sizeof(long) * a);
    for (i = 0; i < a; i++)
        scanf("%ld", &(*arr)[i]);
    return a;
}

void readData(long *k, long *m, long *n, long **a, long **x, long **y)
{
    *k = initArray(a);
    *m = initArray(x);
    *n = initArray(y);
}

void setDataInfo(long k, long m, long n, struct dataInfo *datai)
{
    datai -> k = k;
    datai -> m = m;
    datai -> n = n;
    datai -> sk = 0;
    datai -> sm = k;
    datai -> sn = m + k;
    datai -> sdat = k  + m  + n;
    datai -> datLen = m + n;   
    datai -> totLen = k + m + n + datai -> datLen;
}

void errHandleShm(long *data, int shmID, int eval)
{
    char buf[80];
    if (eval != -1)
        return;
    if (data != NULL)
        shmdt((void *) data);
    if (shmID > -1)
        shmctl(shmID, IPC_RMID, NULL);

    sprintf(buf, "*** MAIN: %s\n", "shared mem error...cleaning up + exiting");
    write(2, buf, strlen(buf));
    exit(1);
}

/* Creates shared memory and returns its id */
int createShm(long **data, struct dataInfo *datai)
{
    int shmID = 0;
    char buf[80];
    key_t key;
    char *msg = "*** MAIN: ";   

    /* Get the key and print it out */
    key = ftok("./", 'j');
    sprintf(buf, "%sshared memory key = %d\n", msg, key);
    write(1, buf, strlen(buf));
    errHandleShm(*data, -1, key);

    /* Create the shared memory */
    shmID = shmget(key, sizeof(long) * datai -> totLen, IPC_CREAT | 0666);
    errHandleShm(*data, shmID, shmID);
    sprintf(buf, "%sshared memory created\n", msg);
    write(1, buf, strlen(buf));
 
    /* Attach to the shared memory */
    *data = (long *) shmat(shmID, NULL, 0);
    errHandleShm(*data, shmID, (int) *data);
    sprintf(buf, "%sshared memory attached and is ready to use\n", msg);
    write(1, buf, strlen(buf));
   
     return shmID;
}

void copyData(long *a, long *x, long *y, struct dataInfo *datai, long **data)
{
    long i = 0;
    for (i = datai -> sk; i < datai -> k; i++)
        (*data)[i] = a[i];
    for (i = 0; i < datai -> m; i++)
        (*data)[i + datai -> sm] = x[i];
    for (i = 0; i < datai -> n; i++)
        (*data)[i + datai -> sn] = y[i];
    for (i = 0; i < datai -> datLen; i++)
        (*data)[i + datai -> sdat] = 0;
}
 
void freeArrays(long *a, long *x, long *y)
{
    free(a);
    free(x);
    free(y);
}

/* Returns the shared memory id */
int loadData(long **data, struct dataInfo *datai)
{
    long k = 0, m = 0, n = 0;
    long *a = NULL, *x = NULL, *y = NULL;
    int shmID = 0;

    readData(&k, &m, &n, &a, &x, &y);
    setDataInfo(k, m, n, datai);
    shmID = createShm(data, datai);
    copyData(a, x, y, datai, data);
    freeArrays(a, x, y);
    return shmID; 
}

int detachRemMem(long *data, int shmID)
{
    int res = shmdt((void *) data);
    shmctl(shmID, IPC_RMID, NULL);
    return res;
}

void printArray(long *arr, long start, long len, char *msg, char *val)
{
    long i = 0;
    char buf[80];
    sprintf(buf, msg, val, len);
    write(1, buf, strlen(buf));
    sprintf(buf, "%3s", "");
    write(1, buf, strlen(buf));
    for (i = 0; i < len; i++)
    {
        sprintf(buf, " %ld ", arr[start + i]);
        write(1, buf, strlen(buf));
    }
    sprintf(buf, "%s\n\n", "");
    write(1, buf, strlen(buf));
}

void setArgsQsort(char *qargs[5], long l, long r, int id, long len)
{
    /*char zero[] = {"./qsort"};
    char one [80];
    char two[80];
    char three[80];
    char four[80];
    sprintf(one, "%ld", l);
    sprintf(two, "%ld", r);
    sprintf(three, "%d", id);
    sprintf(four, "%ld", len);
    args[0] = zero;
    args[1] = one;
    args[2] = two;
    args[3] = three;
    args[4] = four;*/

    int i = 0;
    for (i = 0; i < 5; i++)
        qargs[i] = malloc(sizeof(char) * 80);

    sprintf(qargs[0], "%s", "./qsort");
    sprintf(qargs[1], "%ld", l);
    sprintf(qargs[2], "%ld", r);
    sprintf(qargs[3], "%d", id);
    sprintf(qargs[4], "%ld", len);
    qargs[5] = '\0';
}

void setArgsMsort(char *margs[10], struct dataInfo *datai, int shmID)
{
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        margs[i] = malloc(sizeof(char) * 80);
    }

    sprintf(margs[0], "%s", "./merge");
    sprintf(margs[1], "%ld", datai -> sm);
    sprintf(margs[2], "%ld", datai -> m);
    sprintf(margs[3], "%ld", datai -> sn);
    sprintf(margs[4], "%ld", datai -> n);
    sprintf(margs[5], "%ld", datai -> sdat);
    sprintf(margs[6], "%ld", datai -> datLen);
    sprintf(margs[7], "%d", shmID);
    sprintf(margs[8], "%ld", datai -> totLen);
    margs[9] = '\0';    
}

int main(int argc, char **argv)
{
    char buf[80];
    int shmID = 0;
    int pid1 = 0, pid2 = 0;
    int i = 0;
    int status = 0;
    char * format = "*** MAIN: %s\n";
    long *data = NULL; 
    struct dataInfo *datai = malloc(sizeof(struct dataInfo));
    char *msg = "Quicksort and Binary Merge with Multiple Processes:";
    char *qargs[5];
    char *margs[9];
    int status1, status2;



    /* Print out some info */
    sprintf(buf, "%s\n\n", msg);
    write(1, buf, strlen(buf));
     
    /* Load the data */  
    /*TODO: is shm key to be printed as int or hex? */
    shmID = loadData(&data, datai);

    /* Print the input arrays */
    msg = "Input array for %s has %ld elements";
    printArray(data, datai -> sk, datai -> k, msg, "qsort");
    msg = "Input array %s[] for merge has %ld elements";
    printArray(data, datai -> sm, datai -> m, msg, "x");
    printArray(data, datai -> sn, datai -> n, msg, "y");

    /* Set up the args for qsort and merge */
    setArgsQsort(qargs, 0, datai -> k - 1, shmID, datai -> totLen);
    setArgsMsort(margs, datai, shmID);
 /*   sprintf(qargs[0], "%s", "./qsort");
    sprintf(qargs[1], "%ld", 0);
    sprintf(qargs[2], "%ld", datai -> k - 1);
    sprintf(qargs[3], "%d", shmID);
    sprintf(qargs[4], "%ld", datai -> totLen);
    sprintf(qargs[5], "%c", '\0');*/

    /*sprintf(margs[0], "%s", "./merge");
    sprintf(margs[1], "%ld", datai -> sm);
    sprintf(margs[2], "%ld", datai -> m);
    sprintf(margs[3], "%ld", datai -> sn);
    sprintf(margs[4], "%ld", datai -> n);
    sprintf(margs[5], "%ld", datai -> sdat);
    sprintf(margs[6], "%ld", datai -> datLen);
    sprintf(margs[7], "%d", shmID);
    sprintf(margs[8], "%c", '\0');*/
    
    sprintf(buf, format, "about to spawn the process for qsort");
    write(1, buf, strlen(buf));
    if ((pid1 = fork()) == 0)
    {
        execvp(qargs[0], qargs);
        sprintf(buf, "%s\n", "Qsort: failed to exec properly");
        write(2, buf, strlen(buf));
        return 2;
        sprintf(buf, "%s\n", "failed to exit");
        write(2, buf, strlen(buf));
    }
    else if (pid1 > 0)
    {
        sprintf(buf, format, "about to spawn the process for merge");
        write(1, buf, strlen(buf));
        if ((pid2 == fork()) == 0)
        {
            execvp(margs[0], margs);
            sprintf(buf, "%s\n", "merge: failed to exec properly");
            write(2, buf, strlen(buf));
            
            return 4;
            sprintf(buf, "%s\n", "failed to exit");
            write(2, buf, strlen(buf));
        }  
        else 
        {

            if (pid2 < 0)
            {

                sprintf(buf, "Merge: Error in forking%s\n", "");
                write(2, buf, strlen(buf));
            }
            sprintf(buf, "%s\n", "Main about to wait");
            write(2, buf, strlen(buf));
            /* sleep(20);*/
            wait(&status1);
            wait(&status2);
            sprintf(buf, format, "sorted array by qsort:");
            write(1, buf, strlen(buf));
            sprintf(buf, format, "merged array:");
            write(1, buf, strlen(buf));
        } 
    }
    else 
    {
        sprintf(buf, "%s\n", "Qsort fork error");
        write(1, buf, strlen(buf));
    }
    sprintf(buf, "%s\n", "out of forks");
    write(2, buf, strlen(buf));
    for (i = 0; i < 5; i++)
        free(qargs[i]);
    for (i = 0; i < 9; i++)
        free(margs[i]);
    sprintf(buf, "%s\n", "freeing complete");
    write(2, buf, strlen(buf));
    detachRemMem(data, shmID);

    return 0;
}
