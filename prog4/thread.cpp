/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                   Username: selarkin   */
/* DUE DATE: 03 APRIL 2020                                                   */
/* PROGRAM ASSIGNMENT 4                                                      */
/* FILE NAME: thread.cpp                                                     */
/* PROGRAM PURPOSE:                                                          */
/*    Initializes classes for the landlord-students  simulation.             */
/*-------------------------------------------------------------------------- */

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/* FUNCTION: Student                                                         */
/*    Constructor for the student class                                      */
/* PARAMETER USAGE:                                                          */
/*    sid:  the student's id number                                          */
/*    r: room containing the synchronization information                     */
/* FUNCTION CALLED: NONE                                                     */
/* ------------------------------------------------------------------------- */
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
    sprintf(msg, smsg, "", sid);
    id = sid; 
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: ThreadFunc                                                      */
/*     Starts the student thread running                                     */
/* PARAMETER USAGE:  NONE                                                    */
/* FUNCTION CALLED:                                                          */
/*      printout: utility prints messages with a lock on stdout              */
/*      GoToParty:  student parties in a critical section room               */
/* ------------------------------------------------------------------------- */
void Student::ThreadFunc() {
    Thread::ThreadFunc();
    printout(p0);
    
    /* This student sure parties a lot! */
    while (1)
    {
        Delay();
        GoToParty();
        Delay();
    }
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: Landlord                                                        */
/*    Constructor for the landlord class                                     */
/* PARAMETER USAGE:                                                          */
/*    m: the number of times to check the room before retirement             */
/*    n: the room capacity for safe parties                                  */
/*    k: the number of students living in the appartment                     */
/*    r: room containing the synchronization information                     */
/* FUNCTION CALLED: NONE                                                     */
/* ------------------------------------------------------------------------- */
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
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: ThreadFunc                                                      */
/*     Starts the landlord thread running                                    */
/* PARAMETER USAGE:  NONE                                                    */
/* FUNCTION CALLED:                                                          */
/*      printout: utility prints messages with a lock on stdout              */
/*      CheckRoom:  landlord checks room for too many students and throws    */
/*                  them out if there are too many                           */
/* ------------------------------------------------------------------------- */
void Landlord::ThreadFunc()
{
    Thread::ThreadFunc();
    int i = 0;
    printout(p0, -1);  

    /* Landlord checks the room m times before retiring */
    for (i = 0; i < m; i++)
    {
        Delay();
        //for (int j = 0; j < 10; j++)
          //  Delay();
        CheckRoom(i);
        Delay();
    }   
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: Room                                                            */
/*    Constructor for the room class creates all the synchronization         */
/* PARAMETER USAGE: NONE                                                     */
/* FUNCTION CALLED: NONE                                                     */
/* ------------------------------------------------------------------------- */
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
  
}

