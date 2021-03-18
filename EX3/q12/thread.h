#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThreadClass.h"

class Room {
    public:
        Semaphore *mutex;
        Semaphore *sout;
        Semaphore *custPres;
        Semaphore *barberOpen;
        Semaphore *barberDone;
        Semaphore *chairEmpty;

        int n;
        Room(); 
        void print(char *format);
};

class Barber: public Thread
{
    public:
        Barber(Room *rm);
    private:
        Room *rm;
        int c = 0;
        void ThreadFunc();    
};

class Customer: public Thread
{
    public:
        Customer(Room *rm);
    private:
        Room *rm;
        void ThreadFunc();
};
