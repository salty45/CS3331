/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                      User ID: selarkin */
/* DUE DATE: 03/02/2020                                                      */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: main.c                                                         */
/* PROGRAM PURPOSE:                                                          */
/*    This program forks a child process that executes qsort.c to sort an    */
/*    array using quicksort.  Concurrently, it performs a binary merge on    */
/*    another two arrays by forking a child to execute merge.c.  The arrays  */
/*    are transferred between processes by using shared memory.              */
/* ------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

/* A struct to store a bunch of info on shared memory locations */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION initArray:                                                       */
/*     This function initializes an array of long integers into the given    */
/*     pointer by reading the size followed by the array elements from stdin */
/* PARAMETER USAGE:                                                          */
/*     arr: a pointer to an array of type long * to be filled                */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/* ------------------------------------------------------------------------- */
long initArray(long **arr)
{
    long a = 0, i = 0;
    scanf("%ld", &a);
    *arr = malloc(sizeof(long) * a);
    for (i = 0; i < a; i++)
        scanf("%ld", &(*arr)[i]);
    return a;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION readData:                                                        */
/*    Reads the input file and initializes three temporary arrays on the     */
/*    heap to hold the data until the shared memory is created               */
/* PARAMETER USAGE:                                                          */
/*    k: a pointer to store the length of the first array                    */
/*    m: a pointer to store the length of the second array                   */
/*    n: a pointer to store the length of the third array                    */
/*    a: a pointer to the first array                                        */
/*    x: a pointer to the second array                                       */
/*    y: a pointer to the third array                                        */
/* FUNCTION CALLED:                                                          */
/*    initArray: a utility function to read data from stdin and store to the */
/*               given array                                                 */
/* ------------------------------------------------------------------------- */
void readData(long *k, long *m, long *n, long **a, long **x, long **y)
{
    *k = initArray(a);
    *m = initArray(x);
    *n = initArray(y);
}

/* ------------------------------------------------------------------------- */
/* FUNCTION setDataInfo:                                                     */
/*    Calculates and stores information about the location of each array in  */
/*    the shared memory segment.                                             */
/* PARAMETER USAGE:                                                          */
/*    k: the length of the first array                                       */
/*    m: the length of the second array                                      */
/*    n: the length of the third array                                       */
/*    datai: the struct to store the calculated information                  */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION errHandleShm:                                                    */
/*    Utility function to check for and gracefully handle shared memory      */
/*    errors.                                                                */
/* PARAMATER USAGE:                                                          */
/*    data: the pointer to shared memory                                     */
/*    shmID: the id of the shared memory                                     */
/*    eval: the value to check for equality with -1 (error state)            */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION createShm:                                                       */
/*     Creates a shared memory segment and returns its id                    */
/* PARAMETER USAGE:                                                          */
/*     data: the pointer to the shared memory                                */
/*     datai: a struct holding info about the subarrays stored in shared     */
/*            memory                                                         */
/* FUNCTION CALLED:                                                          */
/*     errHandleShm: error checking utility                                  */
/* ------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION copyData:                                                        */
/*     Copies data from temporary arrays into the given shared memory segment*/
/* PARAMETER USAGE:                                                          */
/*     a: the first temporary array                                          */
/*     x: the second temporary array                                         */
/*     y: the third temporary array                                          */
/*     datai: a struct containing information about the arrays and shared    */
/*            memory segment                                                 */
/*     data: a pointer to the shared memory segment                          */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/*-------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION freeArrays:                                                      */
/*     Frees the three given arrays that held input temporarily              */
/* PARAMETER USAGE:                                                          */
/*     a: the first temporary array                                          */
/*     x: the second temporary array                                         */
/*     y: the third temporary array                                          */
/* FUNCTION CALLED:                                                          */
/*     none                                                                  */
/*-------------------------------------------------------------------------- */
void freeArrays(long *a, long *x, long *y)
{
    free(a);
    free(x);
    free(y);
}

/* ------------------------------------------------------------------------- */
/* FUNCTION loadData:                                                        */
/*    Loads the contents of the three arrays from stdin and stores them into */
/*    temporary arrays before creating the shared memory segment and copying */
/*    the data over. Returns the shared memory id                            */
/* PARAMETER USAGE:                                                          */
/*    data: the pointer to the shared memory segment                         */
/*    datai: a struct storing information about the layout of the shared     */
/*           memory segment                                                  */
/* FUNCTION CALLED:                                                          */
/*    readData: read the data into temporary arrays on the heap              */
/*    setDataInfo: store information about how to organize the shared memory */
/*    createShm: create the shared memory segment                            */
/*    copyData: copy the stored arrays into shared memory                    */
/*    freeArray: free the temporary arrays                                   */
/* ------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION detachRemMem:                                                    */
/*    Detaches from and then removes the given shared memory segement.       */
/*    Returns the result of the detach operation.                            */
/* PARAMETER USAGE:                                                          */
/*    data: the pointer to the shared memory segment                         */
/*    shmID: the id for the shared memory segment                            */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
int detachRemMem(long *data, int shmID)
{
    char buf[80];
    int res = shmdt((void *) data);
    if (res == 0)
    {
        sprintf(buf, "*** MAIN: shared memory successfully detached%s\n", "");
        write(1, buf, strlen(buf));
    }
    if (shmctl(shmID, IPC_RMID, NULL) != -1)
    {
        sprintf(buf, "*** MAIN: shared memory successfully removed%s\n", "");
        write(1, buf, strlen(buf));
    }
    return res;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: printArray                                                      */
/*    Utility function prints out the given array from start to len.         */
/* PARAMETER USAGE:                                       */
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

/* ------------------------------------------------------------------------- */
/* FUNCTION: setArgsQsort                                                    */
/*    Utility function fills the argument array to use when executing qsort  */
/* PARAMETER USAGE:                                                          */
/*    qargs: pointer to the argument array                                   */
/*    l: the index of the left end of the array                              */
/*    r: the index of the rightmost element in the array                     */
/*    id: the id of the shared memory segment                                */
/*    len: the total length of the shared memory segment                     */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
void setArgsQsort(char *qargs[5], long l, long r, int id, long len)
{
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

/* ------------------------------------------------------------------------- */
/* FUNCTION setArgsMsort:                                                    */
/*    Utility function fills the argument array to use when executing merge  */
/* PARAMETER USAGE:                                                          */
/*    margs: pointer to the argument array                                   */
/*    datai: info on the arrays start and end points                         */
/*    shmID: shared memory id                                                */
/* FUNCTION CALLED:                                                          */
/*    none                                                                   */
/* ------------------------------------------------------------------------- */
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


long countArr(long *arr, long start, long end)
{
    long i = 0;
    long arrLen = 0;
    char buf[12];
    for (i = start; i <= end; i++)
    {
        sprintf(buf, "%ld ", arr[i]);
        arrLen += strlen(buf);
    }
    return arrLen;
}

/* ------------------------------------------------------------------------- */
/* */
void printArray2(long *arr, long start, long end, char *msg)
{
    long arrLen = countArr(arr, start, end);
    char buf[120];
    long length = 0;
    char *printout;
    long i;
    sprintf(buf, "%s", msg);
    length = arrLen + strlen(buf) + 3 + 5;
    printout = malloc(sizeof(char) * length);
    sprintf(printout, "%s", "");
    strcat(printout, buf);
    sprintf(buf, "%5s", "");
    strcat(printout, buf);
    for (i = start; i <= end; i++)
    {
        sprintf(buf, "%ld ", arr[i]);
        strcat(printout, buf);
    }
    strcat(printout, "\n\n");
    write(1, printout, strlen(printout));
    free(printout);
}

void freeArgArray(char *arr[], int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
        free(arr[i]);
}

/* ------------------------------------------------------------------------- */
/* FUNCTION main:                                                            */
/*    Main reads in the input, creates the shared memory, forks two children */
/*    to execute the programs qsort and merge. It then waits for its child   */
/*    processes to complete before printing the results and cleaning up      */
/*    shared memory                                                          */
/* PARAMETER USAGE:                                                          */
/*    argc: the number of command line arguments                             */
/*    argv: array of command-line arguments                                  */
/* FUNCTION CALLED:                                                          */
/*    loadData: loads data into shared memory from stdin                     */
/*    printArray: prints out the given array                                 */
/*    setArgsQsort: utility creates the arguments to execute qsort           */
/*    setArgsMsort: uitlity creates the arguments to execute binary merge    */
/*    detachRemMem: detach from and remove shared memory                     */
/* ------------------------------------------------------------------------- */
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
    int wait1, wait2;

    /* Print out some info */
    sprintf(buf, "%s\n\n", msg);
    write(1, buf, strlen(buf));

    /* Load the data */
    shmID = loadData(&data, datai);


    /* Print the input arrays */
    msg = "Input array for %s has %ld elements:\n";
    printArray(data, datai -> sk, datai -> k, msg, "qsort");
    msg = "Input array %s[] for merge has %ld elements:\n";
    printArray(data, datai -> sm, datai -> m, msg, "x");
    printArray(data, datai -> sn, datai -> n, msg, "y");


    /* Set up the args for qsort and merge */
    setArgsQsort(qargs, 0, datai -> k - 1, shmID, datai -> totLen);

    setArgsMsort(margs, datai, shmID);

    sprintf(buf, format, "about to spawn the process for qsort");
    write(1, buf, strlen(buf));

    /* Perform the forks */
    if ((pid1 = fork()) == 0)
    {

        status1 = execvp(qargs[0], qargs);

        sprintf(buf, "*** MAIN: %s\n", "qsort failed to exec properly");
        write(2, buf, strlen(buf));
        exit(1);
    }
    if (pid1 < 0)
    {
        sprintf(buf, "*** MAIN: Forking error: %s\n", strerror(errno));
        write(2, buf, strlen(buf));
    }

    sprintf(buf, format, "about to spawn the process for merge");
    write(1, buf, strlen(buf));
    if ((pid2 = fork()) == 0)
    {
        execvp(margs[0], margs);
        sprintf(buf, "*** MAIN: %s%s\n", "merge: failed to exec properly ",
        strerror(errno));
        write(2, buf, strlen(buf));
        exit(1);
    }

    if (pid2 < 0)
    {
        sprintf(buf, "*** MAIN: Forking error: %s\n", strerror(errno));
        write(2, buf, strlen(buf));
    }

    /* Wait for child processes */
    wait1 = wait(&status1);
    wait2 = wait(&status2);

    /* Print out the sorted arrays */
    sprintf(buf, format, "sorted array by qsort:");
    printArray2(data, datai -> sk, datai -> k - 1, buf);

    sprintf(buf, format, "merged array:");
    printArray2(data, datai -> sdat, datai -> sdat + datai -> datLen - 1, buf);

    /* Free the argument arrays */
    freeArgArray(qargs, 5);
    freeArgArray(margs, 9);
    free(datai);

    detachRemMem(data, shmID);

    return 0;
}
