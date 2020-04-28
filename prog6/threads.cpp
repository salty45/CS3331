#include "thread.h"
#include <string.h>

public Master::Master () {


}

void Seive::ThreadFunc()
{
    Thread::ThreadFunc();
    int k = 0;
    while (k != -1)
    {   
        from.recieve(&k, sizeof(int));
        if (k == -1) {
            // end the program
        } else if (k % i == 0) {
            printOut(4, k);
        } else if (next == 0) {

        } else if (
        //send(&k, sizeof(int));       
    } 
    Exit();
}

public Seive::Seive(int *arr, int id, int i)
{
    Primes = arr; 
    UserDefinedThreadID = id;
    idx = i;
    next = NULL;
    //k = 2;
    in = new SynOneToOneGhannel(name, prev, id);
    out = NULL;
}

public Master::ThreadFunc() 
{
    Thread::ThreadFunc();
    
    Exit();
}

void Seive::PrintOut(int i, int k = 0)
{
    char buf[80];
    char space[80];
    int j = 0;
    for (j = 1; j < idx; j++)
    {
        sprintf(space, "%2s", "");
        strcat(buf, space);
    }
    if (i == 1)
        sprintf(space, "P%d starts and memorizes %d\n", id, id);
    else if (i == 2)
        sprintf(space, "P%d receives %d\n", id, k);
    else if (i == 3)
        sprintf(space, "P%d creates P%d\n", id, k);
    else if (i == 4)
        sprintf(space, "P%d ignores %d\n", id, k);
    else if (i == 5)
        sprintf(space, "P%d sends %d to %d\n", id, k, next);  
    strcat(buf, space);
    write(1, buf, strlen(buf));
}
