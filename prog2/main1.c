/* ------------------------------------------------------------------------- */
/* NAME : Sarah Larkin                                     User ID: selarkin */
/* DUE DATE: 02/24/2020                                                      */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME : main.c                                                        */
/* PROGRAM PURPOSE :                                                         */
/*    This program follows the specs given in assignment 2                   */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char **argv)
{
/* TODO: how big is the maximum number I have to deal with for reading in?
 * i.e. how many base 10 digits? */
    long k, m, n;
    long ki = 0;
    long mi = 0;
    long ni = 0;
    char buf[80];
    char c;
    char in[11];
    long i = 0;
    long numPerLine = 0;
    int lineNum = 0;
   
    long *a = NULL, *x = NULL, *y = NULL;
     
    /* Read the data in from stdin which has received a data file */
    while (read(0, &c, 1) != 0)
    {
        printf("C = %c\n", c);
        if ( c >= '0' && c <= '9')
        {
            /* Add c to the in array and increment counter */
            in[i++] = c;
            /* printf("In w c: %s\n", in); */
        }
        else 
        {
         /*   printf("In w space: %s\n", in);*/
            /* Not a number means a space or \n */
           
            /* Parse the in array to an int/long and store in array */
            switch(lineNum)
            {
                case 0:
                    k = atol(in);
                    break;
                case 1:
                    if (a == NULL)
                        a = (long *) malloc(sizeof(long) * k);
                    a[ki++] = atol(in);
             /*       printf("a[%d]: %d %s \n", ki - 1, a[ki - 1], in);*/
                    break;
                case 2:
                    m = atol(in);
                    break;
                case 3:
                    if (x == NULL)
                    {
                        x = (long *) malloc(sizeof(long) * m);
                     /*   printf("make x: %d\n", x);*/
                    }
                    x[mi++] = atol(in);
                    break;
                case 4:
                    n = atol(in);
                    break;
                case 5:
                    if (y == NULL)
                    {
                        y = (long *) malloc(sizeof(long) * m);
                     /*   printf("make y: %d\n", y);*/
                    }
                    y[ni++] = atol(in);
                    break;
            }
              
            /* Then reset in and counter */
            i = 0;
            for (i = 0; i < 11; i++)
            {
                in[i] = 0;
            }                       
            i = 0;
            if (c == '\n')
                lineNum++; 
        } 
    }
    printf("%s\n", "After loop");
    for (i = 0; i < k; i++)
    {
        sprintf(buf, "%ld ", a[i]);
        write(1, buf, strlen(buf));
    }
    sprintf(buf, "%s\n", "");
    write(1, buf, strlen(buf));        
    for (i = 0; i < m; i++)
    {
        /*printf("%d\n", x == NULL);
        printf("hello: %d\n", x);*/
        sprintf(buf, "%ld ", x[i]);
        write(1, buf, strlen(buf));
    }
    sprintf(buf, "%s\n", "");
    write(1, buf, strlen(buf));        
    for (i = 0; i < n; i++)
    {
        sprintf(buf, "%ld ", y[i]);
        write(1, buf, strlen(buf));
    }        
    sprintf(buf, "%s\n", "");
    write(1, buf, strlen(buf));
    free(a);
    a = NULL;
    free(x);
    x = NULL;
    free(y);
    y = NULL;
    sprintf(buf, "%s\n", "Done");
    write(1, buf, strlen(buf));
    return 0;
}
