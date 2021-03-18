/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                   Username: selarkin   */
/* DUE DATE: 03 APRIL 2020                                                   */
/* PROGRAM ASSIGNMENT 4                                                      */
/* FILE NAME: thread.h                                                       */
/* PROGRAM PURPOSE:                                                          */
/*    Header file declares classes for the landlord-students simulation.     */
/*-------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThreadClass.h"

class Room
{
    public:
        Semaphore *studentQueue;
        Semaphore *door;
        Semaphore *entry;
        Semaphore *studentCount;
        Semaphore *egress;
        Semaphore *exited;
        Semaphore *bahamas;
        Semaphore *sout;
        Semaphore *allDone;

        bool landlordRetired;
        bool landlordPresent;
        int numStudents;
        
        int studentsWaiting;     
        Room();
       
};

class Landlord: public Thread
{

    public:
       Landlord(int m, int n, int k, Room *r);
    private:
        void ThreadFunc();
        void CheckRoom(int i);
        void printout(char *format, int n = -1);
        void kickStudentsOut();
        void retire();
        Semaphore *studentQueue;
        Semaphore *door;
        Semaphore *entry;
        Semaphore *studentCount;
        Semaphore *egress;
        Semaphore *exited;
        Semaphore *bahamas;
        Semaphore *sout;
        Semaphore *allDone;
        bool *landlordRetired;
        bool *landlordPresent;
        int *numStudents;
        int *StudentsWaiting;             
        int m = 0; 
        int n = 0;
        int totalStudents;

        char buf[80];
        char *s = "The landlord";
        char *p0 = "The landlord starts%s\n";
        char *p1 = "The landlord checks the room the %d-th time\n";
        char *p2 = "The landlord finds the room has no students and leaves%s\n";
        
        char *p3 = "The landlord finds there are %d students in the room "
                   "(condition good) and leaves\n";
        char *p4 = "The landlord finds there are %d students in the room "
                   "and breaks up the party\n";
        char *p5 = "The landlord finishes checking and forces everyone "
                   "to leave%s\n";
        char *p6 = "The landlord leaves the room and the room is empty%s\n";
        char *p7 = "After checking the room %d times, the landlord retires " 
                   "and is on vacation!\n";
};

class Student: public Thread
{
    public:
       Student(int sid, Room *r);
    private:
        void ThreadFunc();
        void printout(char *format, int n = -1);
        void GoToParty();
        void enterRoom();
        Semaphore *studentQueue;
        Semaphore *door;
        Semaphore *entry;
        Semaphore *studentCount;
        Semaphore *egress;
        Semaphore *exited;
        Semaphore *bahamas;
        Semaphore *sout;
        Semaphore *allDone;

        bool *landlordRetired;
        bool *landlordPresent;
        int *numStudents;
        int *StudentsWaiting;     

        int id;
        char msg[16];
        char *smsg = "%5sStudent %2d";
        char *p0 = "%s starts\n";
        char *p1 = "%s waits to enter the room\n";
        char *p2 = "%s enters the room (%2d students in the room)\n";
        char *p3 = "%s waits to leave the room\n";
        char *p4 = "%s leaves the room (%2d students in the room)\n";
        char *p5 = "%s terminates\n";
        
};


