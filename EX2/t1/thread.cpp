#include "thread.h"

State::State()
{
    onew = new Semaphore("ow", 0);
    wait = new Semaphore("wait", 0);
    sout = new Semaphore("sout", 1);
//    mutex = new Semaphore("m", 1);
    n1w = 0;
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
    Thread::ThreadFunc();
    
    state->sout->Wait();
    state->n1w++;
    printf("Thread %d waits: wt = %d\n", id, state->n1w);
    if (state->n1w > 3 )
        state->wait->Signal();
    state->sout->Signal();
    state->onew->Wait();   
    printf("Thread %d is signaled\n", id);
    state->wait->Signal();
    Exit();
}

void P2::ThreadFunc()
{
    int i = 0;
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
    Exit();      
}

