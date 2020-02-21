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


int main(int argc, char **argv)
{
    char buf[80];
    int shmID = 0;
    int pid = 0;
    int status = 0;
    char * format = "*** MAIN: %s\n";
    long *data = NULL; 
    struct dataInfo *datai = malloc(sizeof(struct dataInfo));
    char *msg = "Quicksort and Binary Merge with Multiple Processes:";
    
    /* Print out some info */
    sprintf(buf, "%s\n\n", msg);
    write(1, buf, strlen(buf));
    
    /* Load the data */  
    shmID = loadData(&data, datai);

    /* Print the input arrays */
    msg = "Input array for %s has %ld elements";
    printArray(data, datai -> sk, datai -> k, msg, "qsort");
    msg = "Input array %s[] for merge has %ld elements";
    printArray(data, datai -> sm, datai -> m, msg, "x");
    printArray(data, datai -> sn, datai -> n, msg, "y");

    detachRemMem(data, shmID);

    return 0;
}
