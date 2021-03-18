#include "thread.h"

State::State()
{
    mutex = new Semaphore("ow", 1);
    fentry = new Semaphore("wait", 0);
    mentry = new Semaphore("sout", 0);
//    mutex = new Semaphore("m", 1);
    n1w = 0;
    femp = 0;
    menp = 0;
    femw = 0;
    menw = 0;
}

Male::Male(int i, State *st)
{
    id = i;
    state = st;
}

Female::Female(int i, State *st)
{
    id = i;
    state = st;
}

void Male::ThreadFunc()
{
    Thread::ThreadFunc();
    state->mutex->Wait();
    if (state->femp == 0)
    {
        state->menp++;
        printf("Male %d enters without waiting\n", id);
        state->mutex->Signal();
    }   
    else
    {
        state->menw++;
        printf("Male %d waits; %d women present\n", id, state->femp);
        state->mutex->Signal();
        state->mentry->Wait();
        printf("male %d enters: ...\n", id);
    }

    int j = 0;
    for (j = 0; j < 500 * id; j++)
        Delay();// CS

    state->mutex->Wait();
    state->menp--;
    state->menw = 0;
    printf("Male %d exits\n", id);
    if (state->menp == 0)
    {
        int i = 0;
        for (i = 0; i < state->femw; i++)
        {   
            state->femp++;
            state->fentry->Signal();
        }
    }
        state->mutex->Signal();
    Exit();
}

void Female::ThreadFunc()
{
    int i = 0;
    Thread::ThreadFunc();
    state->mutex->Wait();
    if (state->menp == 0)
    {
        state->femp++;
        printf("female %d enters without waiting\n", id);
        state->mutex->Signal();
    }   
    else
    {
        state->femw++;
        printf("female %d waits; %d men present\n", id, state->menp);
        state->mutex->Signal();
        state->fentry->Wait();
        printf("female %d enters: ...\n", id);
    }
    int j = 0;
    for (j = 0; j < 500; j++)
        Delay();// CS

    state->mutex->Wait();
    state->femp--;
    state->femw = 0;
    printf("female %d exits\n", id);
    if (state->femp == 0) {
        int i = 0;
        for (i = 0; i < state->menw; i++)
        {   
            state->menp++;
            state->mentry->Signal();
        }
    }
        state->mutex->Signal();
    
    Exit();      
}

