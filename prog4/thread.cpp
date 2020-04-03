#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

/*Student::Student(Semaphore *sem) {
    s = sem;
}*/
/*Student::Student(Semaphore *sq, Semaphore *d, Semaphore *e, Semaphore *sc,
                Semaphore *eg, Semaphore *ex, Semaphore *b, Semaphore *sy,
                bool *lr, bool *lp, int *ns, int sid, Room *r)*/
Student::Student(int sid, Room *r)
{
    studentQueue = r->studentQueue;
    door = r->door;
    entry = r->entry;
    studentCount = r->studentCount;
    egress = r->egress;
    exited = r->exited;
    bahamas = r->bahamas;
    landlordRetired = &r->landlordRetired;
    landlordPresent = &r->landlordPresent;
    numStudents = &r->numStudents;
    allDone = r->allDone;
    sout = r->sout;
    StudentsWaiting = &r->studentsWaiting;
/*tudentQueue = sq;
    door = d;
    entry = e;
    studentCount = sc;
    egress = eg;
    exited = ex;
    bahamas = b;
    landlordRetired = lr;
    landlordPresent = lp;
    numStudents = ns;

    sout = sy;   */
    sprintf(msg, smsg, "", sid);
    id = sid; 
}

void Student::ThreadFunc() {
    Thread::ThreadFunc();
    printout(p0);
    while (1)
    {
        Delay();
        GoToParty();
        Delay();
    }
}

/*Landlord::Landlord(Semaphore *sq, Semaphore *d, Semaphore *e, Semaphore *sc,
                Semaphore *eg, Semaphore *ex, Semaphore *b,
                bool *lr, bool *lp, int *ns, int m, int n, Room *r)*/
Landlord::Landlord(int m, int n, int k, Room *r)
{
    studentQueue = r->studentQueue;
    door = r->door;
    entry = r->entry;
    studentCount = r->studentCount;
    egress = r->egress;
    exited = r->exited;
    bahamas = r->bahamas;
    landlordRetired = &r->landlordRetired;
    landlordPresent = &r->landlordPresent;
    numStudents = &r->numStudents;
    allDone = r->allDone;
    sout = r->sout;
    this->m = m;   
    this->n = n;
    StudentsWaiting = &r->studentsWaiting;
    totalStudents = k;
    printf("t->m %d m: %d t->n %d n: %d\n", this->m, m, this->n, n);
    printf("ns: %d\n", *numStudents);
}

void Landlord::ThreadFunc()
{
    Thread::ThreadFunc();
    printf("hello there!\n");
    fflush(stdout);
    int i = 0;
    printout(p0, -1);   
    printf("Whoo!\n");
    fflush(stdout);
    for (i = 0; i < m; i++)
    {
        Delay();
        CheckRoom(i);
        Delay();
    }   
}

Room::Room()
{
    studentQueue = new Semaphore("sq", 1);
    door = new Semaphore("d", 1);
    entry = new Semaphore("e", 1);
    egress = new Semaphore("eg", 1);
    exited = new Semaphore("ex", 1);
    studentCount = new Semaphore("sc", 1);
    sout = new Semaphore("sout", 1);
    bahamas = new Semaphore("b", 1);
    allDone = new Semaphore("ad", 0);
    landlordRetired = false;
    landlordPresent = false;
    numStudents = 0;
    studentsWaiting = 0; 
    /*studentQueue = sq;
    door = d;
    entry = e;
    studentCount = sc;
    egress = eg;
    exited = ex;
    bahamas = b;
    allDone = ad;
    sout = s;

    landlordRetired = lr;
    landlordPresent = lp;
    numStudents = ns;
    printf("rns: %d\n", *numStudents);
    (*landlordRetired) = false;
    (*landlordPresent) = false;
    (*numStudents) = 0;
 */   
}

