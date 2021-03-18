#include "thread.h"

Room::Room()
{
    mutex = new Semaphore("mutex", 1);
    sout = new Semaphore("sout", 1);
    custPres = new Semaphore("cp", 0);
    barberOpen = new Semaphore("bo", 0);
    barberDone = new Semaphore("bd", 0);
    chairEmpty = new Semaphore("ce", 0);

    n = 5;
}

void Room::print(char *format)
{
    sout->Wait();
    printf("%s\n", format);
    sout->Signal();
}

Barber::Barber(Room *rm)
{
    this->rm = rm;
    c = 0;
}

Customer::Customer(Room *rm)
{
    this->rm = rm;
}

void Barber::ThreadFunc()
{
    Thread::ThreadFunc();
    bool b = false;
    while (true) {
        rm->print("Waiting for customers...");
        rm->custPres->Wait();
        rm->print("Come on back!");
        rm->barberOpen->Signal();
        rm->print("cutting hair");
        Delay();//CUT hair
        rm->chairEmpty->Wait();   
        rm->print("all done");    
        rm->mutex->Wait();
        if (++c > 0 && rm->n == 5)
            b = true;
        rm->mutex->Signal();
        if (b)
            Exit();
    }
    Exit();
}

void Customer::ThreadFunc()
{
    Thread::ThreadFunc();
    rm->mutex->Wait();
    if (rm->n > 0) {
        rm->print("hey barber, i'm herre!");
        rm->custPres->Signal();
        rm->n--;
        rm->print("sitting down to wait");
        rm->mutex->Signal();
        //rm->print("hey barber, i'm herre!");
        //rm->custPres->Signal();
        rm->barberOpen->Wait();
        rm->print("commin");
        rm->mutex->Wait();
        rm->n++;
        rm->print("chair emptied");
        rm->mutex->Signal();
        rm->print("buzzer");
        Delay();
        Delay();
        rm->print("i'm leavin");
        rm->chairEmpty->Signal();
    } else {
        rm->mutex->Signal();
    }
    Exit();
}
