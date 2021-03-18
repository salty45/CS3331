#include "thread.h"

State::State(int n)
{
    n1w = n;
 //   onew = new Semaphore("ow", 0);
   // wait = new Semaphore("wait", 0);
    //out = new Semaphore("sout", 1);
//    mutex = new Semaphore("m", 1);
    arr = (int *) malloc(sizeof(int) * n);
    b = (int *) malloc(sizeof(int) * n);
    //sems = (Semaphore **) malloc(sizeof(Semaphore*) * n); 
    n1w = 0;
    int i = 0;
    Semaphore *s0 = new Semaphore("s0", 1);
    Semaphore *s1 = new Semaphore("S1", 1);
    Semaphore *s2 = new Semaphore("s2", 1);
    Semaphore *s3 = new Semaphore("s3", 1);
    sems[0] = s0;
    sems[1] = s1;
    sems[2] = s2;
    sems[3] = s3;
    for (i = 0; i < 4; i++)
    {
        //char buf[80];
     //   sprintf(buf, "%c%d", 'i', i);
       // sems[i] = Semaphore(buf, 1);
        arr[i] = i;
        b[i] = 0;
    }
}

void State::printOut()
{
    int i = 0;
    printf("Array arr:\n  [");
    for (i = 0; i < 4; i++)
        printf(" %d ", arr[i]);
    printf("]\n\n Array b:\n [");
    for (i = 0; i < 4; i++)
        printf(" %d ", b[i]);
    printf("]\n\n");
}   

P1::P1(int i, State *st)
{
    id = i;
    state = st;
}

P2::P2(int i, State *st)
{
    id = i;
    state = st;
}

void P1::ThreadFunc()
{
    int i = id;
    Thread::ThreadFunc();
    state->sems[i-1]->Wait();
    state->sems[i]->Wait();
    state->arr[i] = state->arr[i] + state->arr[i-1];
    state->sems[i]->Signal();
    state->sems[i - 1]->Signal();
    state->b[i] = state->arr[i] - 1;
    /*state->sout->Wait();
    state->n1w++;
    printf("Thread %d waits: wt = %d\n", id, state->n1w);
    if (state->n1w > 3 )
        state->wait->Signal();
    state->sout->Signal();
    state->onew->Wait();   
    printf("Thread %d is signaled\n", id);
    state->wait->Signal();*/
    Exit();
}

void P2::ThreadFunc()
{
    /*int i = 0;
    Thread::ThreadFunc();
    state->sout->Wait();
    printf("T2 running\n");
    state->sout->Signal();
    for ( i = 0; i < 4; i++) {
        state->wait->Wait();
        state->sout->Wait();
        state->n1w--;
        printf("numWaiting: %d\n", state->n1w);
        state->onew->Signal();
        state->sout->Signal();
    }
    Exit();      */
}

