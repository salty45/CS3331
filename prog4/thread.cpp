#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

Student::Student(Semaphore *sem) {
    s = sem;
}
Student::Student(Semaphore *sq, Semaphore *d, Semaphore *e, Semaphore *sc,
                Semaphore *eg, Semaphore *ex, Semaphore *b, Semaphore *sy,
                bool lr, bool lp, int ns, int sw, Room *r)
{
    studentQueue = sq;
    door = d;
    entry = e;
    studentCount = sc;
    egress = eg;
    exited = ex;
    bahamas = b;
    landlordRetired = lr;
    landlordPresent = lp
    numStudents = ns;
    studentsWaiting = sw;
    sout = sy;   

}

void Student::ThreadFunc() {
    Thread::ThreadFunc();
    s->Wait();
    printf("hello\n");
    s->Signal();
    Exit();
}
