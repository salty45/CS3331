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
    int res = shmdt((void *) data);
    shmctl(shmID, IPC_RMID, NULL);
    return res;
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: printArray                                                      */
/*    Utility function prints out the given array from start to len.         */
/* PARAMETER USAGE:         */
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
    long len = end - start + 1;
    char buf[12];
    for (i = start; i <= end; i++)
    {
        sprintf(buf, "%ld ", arr[i]);
        arrLen += strlen(buf);
    }
    return arrLen;
}

void printArray2(long *arr, long start, long end, char *msg)
{
    long arrLen = countArr(arr, start, end);
    printf("arr len: %ld\n", arrLen);
 /*   char *msg = "%3s### Q-PROC(%4d): entering with a[%ld..%ld]\n";*/
    char buf[120];
    long length = 0;
    char *printout;
    long i;
   /* printf("What %ld\n", arrLen);*/
   /* getpid();
    printf("Works!\n");
    sprintf(buf, msg, "", getpid(), start, end);
    printf("%s\n", buf);*/
    sprintf(buf, "%s", msg);
    length = arrLen + strlen(buf) + 3 + 7;
   /* printf("length: %ld\n", length);*/
    printout = malloc(sizeof(char) * length);
   /* printf("hello\n");*/
    sprintf(printout, "%s", "");
    strcat(printout, buf);
/*    printf("%s\n", printout);*/
    sprintf(buf, "%7s", "");
    strcat(printout, buf);
    for (i = start; i <= end; i++)
    {
        sprintf(buf, "%ld ", arr[i]);
        strcat(printout, buf);
    }
    write(1, printout, strlen(printout));
   /* printf("%s\n", printout);*/
    free(printout);
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


    /* Arguments for exec */
    char *qarg0 = "./qsort" ;
    char qarg1[12];
    char qarg2[12];
    char qarg3[12];
    char qarg4[12];
    char qarg5 = '\0';

    char *marg0 = "./merge";
    char marg1[12];
    char marg2[12];
    char marg3[12];
    char marg4[12];
    char marg5[12];
    char marg6[12];
    char marg7[12];
    char marg8[12];
    char marg9 = '\0';

    /* initialize the arguments to be 0's */
    bzero(qarg1, 12);
    bzero(qarg2, 12);
    bzero(qarg3, 12);
    bzero(qarg4, 12);
    bzero(marg1, 12);
    bzero(marg2, 12);
    bzero(marg3, 12);
    bzero(marg4, 12);
    bzero(marg5, 12);
    bzero(marg6, 12);
    bzero(marg7, 12);
    bzero(marg8, 12);

    qargs[0] = qarg0;
    qargs[1] = qarg1;
    qargs[2] = qarg2;
    qargs[3] = qarg3;
    qargs[4] = qarg4;
    qargs[5] = qarg5;

   /* printf("datum: %ld\n", data);
    i = data[0];
    printf("data c: %ld\n", data[0]);*/
    
    margs[0] = marg0;
    margs[1] = marg1;
    margs[2] = marg2;
    margs[3] = marg3;
    margs[4] = marg4;
    margs[5] = marg5;
    margs[6] = marg6;
    margs[7] = marg7;
    margs[8] = marg8;
    margs[9] = marg9;
 
   
    /* Print out some info */
    sprintf(buf, "%s\n\n", msg);
    write(1, buf, strlen(buf));
     
    /* Load the data */  
    /*TODO: is shm key to be printed as int or hex? */
    shmID = loadData(&data, datai);

    
    /* Print the input arrays */
    msg = "Input array for %s has %ld elements:\n";
    printArray(data, datai -> sk, datai -> k, msg, "qsort");
    msg = "Input array %s[] for merge has %ld elements:\n";
    printArray(data, datai -> sm, datai -> m, msg, "x");
    printArray(data, datai -> sn, datai -> n, msg, "y");

    printf("data e: %ld\n", data[0]);
    printf("data n: %ld\n", data[datai -> sdat]);
    printf("Data : %ld\n", data);
 
    /* Set up the args for qsort and merge */
    /*setArgsQsort(qargs, 0, datai -> k - 1, shmID, datai -> totLen);
    printf("data? %ld\n", data[0]);
    setArgsMsort(margs, datai, shmID);*/
    sprintf(qarg1, "%ld", 0);
    sprintf(qarg2, "%ld", datai -> k - 1);
    sprintf(qarg3, "%d", shmID);
    sprintf(qarg4, "%ld", datai -> totLen);

    sprintf(marg1, "%ld", datai -> sm);
    sprintf(marg2, "%ld", datai -> m);
    sprintf(marg3, "%ld", datai -> sn);
    sprintf(marg4, "%ld", datai -> n);
    sprintf(marg5, "%ld", datai -> sdat);
    sprintf(marg6, "%ld", datai -> datLen);
    sprintf(marg7, "%d", shmID);
    sprintf(marg8, "%ld", datai -> totLen);
   

    for (i = 0; i < 5; i++)
        printf("qargs[%d] = %s\n", i, qargs[i]);
 
/*
    qargs[0] = qarg0;
    qargs[1] = qarg1;
    qargs[2] = qarg2;
    qargs[3] = qarg3;
    qargs[4] = qarg4;
    qargs[5] = qarg5;
*/
    printf("datum: %ld\n", data);
    i = data[0];
    printf("data c: %ld\n", data[0]);
    
  /*  margs[0] = marg0;
    margs[1] = marg1;
    margs[2] = marg2;
    margs[3] = marg3;
    margs[4] = marg4;
    margs[5] = marg5;
    margs[6] = marg6;
    margs[7] = marg7;
    margs[8] = marg8;
    margs[9] = marg9;
*/
    




    

    printf("data a: %ld\n", data[0]);
    printf("data b: %ld\n", data[datai -> sdat]);
 
    
   
    sprintf(buf, format, "about to spawn the process for qsort");
    write(1, buf, strlen(buf));

    /* Perform the forks */
    if ((pid1 = fork()) == 0)
    {
        
        status1 = execvp(qargs[0], qargs);
        i = errno;
        write(1, "EXEC FAILED\n", strlen("EXEC FAILED\n"));
        
        if (i == EACCES)
           write(1, "q-EACCESS error\n", strlen("q-EACCESS error\n"));
        else if (i == EIO)
            write(1, "q-EIO error\n", strlen("q-EIO error\n"));
        else if (i == ENOEXEC)
            write(1, "q-ENOEXEC\n", strlen("q-ENOEXEC\n"));
        else
            strerror(i);
        perror(&i);
        sprintf(buf, "%d\n", i);
        write(1, buf, strlen(buf));   
       
        sprintf(buf, "%s\n", "Qsort: failed to exec properly");
        write(2, buf, strlen(buf));
        return 2;
        sprintf(buf, "%s\n", "failed to exit");
        write(2, buf, strlen(buf));
    }
    if (pid1 < 0)
    {
        sprintf(buf, "Forking error: %s\n", strerror(errno));
        write(2, buf, strlen(buf));
    }
    
    sprintf(buf, format, "about to spawn the process for merge");
    write(1, buf, strlen(buf));
    if ((pid2 = fork()) == 0)
    {
        execvp(margs[0], margs);
        sprintf(buf, "%s%s\n", "merge: failed to exec properly ", 
        strerror(errno));
        write(2, buf, strlen(buf));
        exit(1);
    }  
    
    if (pid2 < 0)
    {
        sprintf(buf, "Merge: Error in forking%s\n", "");
        write(2, buf, strlen(buf));
    }
    sprintf(buf, "%s\n", "Main about to wait");
    write(2, buf, strlen(buf));
     
       
    sprintf(buf, "%s\n", "out of forks");
    write(2, buf, strlen(buf));
    sprintf(buf, "Data - O: %ld\n", data[0]);
    write(1, buf, strlen(buf));
    /* Uncomment the sleep below to get program to stop before wait, since it
     * currently doesn't stop to wait. WHY? */
    /* sleep(30);*/

    sprintf(buf, "wait 1: %d\n", (wait1 = wait(&status1)));
    write(1, buf, strlen(buf));
    sprintf(buf, "wait 2: %d\n", (wait2 = wait(&status2)));
    write(1, buf, strlen(buf));
    sprintf(buf, "err1: %s\n", strerror(status1));
    write(1, buf, strlen(buf));
    sprintf(buf, "err2: %s\n", strerror(status2));
    write(1, buf, strlen(buf));
    if (wait1 == ECHILD)
    {
        
    }
    if (WIFEXITED(status2))
    {
        sprintf(buf, "Exited with status %d\n", WEXITSTATUS(status2));
        write(2, buf, strlen(buf));
    }
    else
    {
        sprintf(buf, "Qsort didn't exit: %s\n", "");
        write(2, buf, strlen(buf));
    }
    if (WIFEXITED(status1)== 0)
    {
        sprintf(buf, "no exit %s\n", "");
        write(1, buf, strlen(buf));
    }
    sprintf(buf, format, "sorted array by qsort:");
    write(1, buf, strlen(buf));
    for (i = datai -> sk; i < datai -> k; i++)
        printf(" %ld ", data[i]);
    printf("%s\n", "");
/*    printArray(data, datai -> sk, datai -> k, msg, "qsort");*/
    sprintf(buf, format, "merged array:");
    write(1, buf, strlen(buf));
    /*printArray(data, datai -> sdat, datai -> datLen, msg, "merge");*/
    printf("%4s", "");
    printf("val data: %ld\n", datai -> sdat);
    printf("data[0] = %ld\n", data[0]);
    /*printArray2(data, 0, datai -> totLen - 1, "abc: \n");*/
    for (i = datai -> sdat; i < datai -> sdat + datai -> datLen; i++)
            printf(" %ld ", data[i]);
    printf("%s\n", "");
    /* Free the argument arrays */
 /*   for (i = 0; i < 5; i++)
        free(qargs[i]);
    for (i = 0; i < 9; i++)
        free(margs[i]);*/
    sprintf(buf, "%s\n", "freeing complete");
    write(2, buf, strlen(buf));
    detachRemMem(data, shmID);

    return 0;
}
