/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                   Username: selarkin   */
/* DUE DATE: 03 APRIL 2020                                                   */
/* PROGRAM ASSIGNMENT 4                                                      */
/* FILE NAME: thread-support.cpp                                             */
/* PROGRAM PURPOSE:                                                          */
/*    Provides synchronization with semaphores for the landlord-students     */
/*    simulation.  Contains methods CheckRoom() and GoToParty()              */
/*-------------------------------------------------------------------------- */


#include "thread.h"

/* ------------------------------------------------------------------------- */
/* FUNCTION: kickStudentsOut                                                 */
/*    Utility function lets landlord signal students to leave in a cascade   */
/*    and waits for the last one to signal that it has left.                 */
/* PARAMETER USAGE:  NONE                                                    */
/* FUNCTION CALLED:                                                          */
/*    printout:  utility that locks and unlocks stdout for printing          */
/* ------------------------------------------------------------------------- */
void Landlord::kickStudentsOut()
{
    printout(p4, *numStudents);
    printout(p5, -1);
    // Begin cascading signals
//    studentCount->Signal();
    egress->Signal();
    studentCount->Signal();
    // Wait for signal from last student
    exited->Wait();
    //exited->Signal(); 
    printout(p6, -1);
}


/* ------------------------------------------------------------------------- */
/* FUNCTION: printout                                                        */
/*    Utility to lock stdout before printing and unlock it after printing.   */
/*    Takes a standardize message and an integer to print.                   */
/* PARAMETER USAGE:                                                          */
/*    format: the message to print                                           */
/*    n: the number of students, or -1 to not print an integer               */
/* FUNCTION CALLED: NONE                                                     */
/* ------------------------------------------------------------------------- */
void Landlord::printout(char *format, int n)
{
    sout->Wait();
    if (n < 0)
        printf(format, "");
    else
        printf(format, n);
    sout->Signal();
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: CheckRoom                                                       */
/*    Lets the landlord access the room as a critical section and determine  */
/*    actions based on the current iteration and number of students present. */
/* PARAMETER USAGE:                                                          */
/*    i: the number of the current iteration                                 */
/* FUNCTION CALLED:                                                          */
/*    printout: utility for printing messages                                */
/*    kickStudentsOut: landlord ejects students and waits for them to leave  */ 
/* ------------------------------------------------------------------------- */
void Landlord::CheckRoom(int i) 
{
    int j = 0;
    /* Entry protocol */
    door->Wait();
    *landlordPresent = true;
    egress->Wait();
    entry->Wait();
    printout(p1, i + 1);
    //door->Signal();//
    /* In CS */
    for (j = 0; j < m * i * 50; j++)
        Delay();

    /* Exit protocol */
    studentCount->Wait();
    
    /* Is it time to retire? */
    if (i == m - 1)
    {
        bahamas->Wait();
        *landlordRetired = true;
        bahamas->Signal();

        kickStudentsOut();
        entry->Signal();
        door->Signal();
        
        /* Wait for all my students to terminate */
        for (j = 0; j < totalStudents; j++)
              allDone->Wait();
        printout(p7, m);
        Exit();
    }
    *landlordPresent = false;

    /* Print out messages based on number of students present */ 
    if (*numStudents == 0)
    {
        printout(p2, -1);
        egress->Signal();
        studentCount->Signal();
    }
    else if(*numStudents <= n)
    {
        printout(p3, *numStudents);
        egress->Signal();
        studentCount->Signal();
    }
    else if(*numStudents > n)
    {
        kickStudentsOut(); 
    }
     door->Signal();
 //   for (j = 0; j < *StudentsWaiting; j++)
        entry->Signal();   
   // *StudentsWaiting = 0;
    //door->Signal();
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: printout                                                        */
/*    Utility to lock stdout before printing and unlock it after printing.   */
/*    Takes a standardize message and an integer to print.                   */
/* PARAMETER USAGE:                                                          */
/*    format: the message to print                                           */
/*    n: the number of students, or -1 to not print an integer               */
/* FUNCTION CALLED: NONE                                                     */
/* ------------------------------------------------------------------------- */
  
void Student::printout(char *format, int n)
{
    sout->Wait();
    if (n < 0)
        printf(format, msg);
    else
        printf(format, msg, n);
    sout->Signal();
}

/* -------------------------------------------------------------------------- */
void Student::enterRoom()
{
    bahamas->Wait();
    if (*landlordRetired)
    {
        bahamas->Signal();
        door->Signal();
        studentQueue->Signal();
        printout(p5);
        allDone->Signal();
        Exit();
    }
    studentCount->Wait();
    (*numStudents)++;
    /* Wait on exited if I'm the first one here.  This makes it so the landlord
     * can tell when the last one has left. */
    if (*numStudents == 1)
    {
        exited->Wait();
    }
    printout(p2, *numStudents);
    studentCount->Signal();
    bahamas->Signal();
    
}

/* ------------------------------------------------------------------------- */
/* FUNCTION: GoToParty                                                       */
/*    Students access the room as a critical section to party in             */
/* PARAMETER USAGE: NONE                                                     */
/* ------------------------------------------------------------------------- */ 
void Student::GoToParty()
{
    printout(p1); 
    studentQueue->Wait();
    door->Wait();
    
    /* Yield if the landlord was at the door first */
    if (*landlordPresent)
    {
        //*StudentsWaiting++;
        door->Signal();// release the door for the landlord
        studentQueue->Signal();// release the queue entry for students
        entry->Wait();// wait for the landlord to leave
        entry->Signal();
        door->Wait();
        
        enterRoom();
        door->Signal();
    }
    else
    {
        enterRoom();
        door->Signal();
        studentQueue->Signal();
    }
   
    /* In CS */
    
    for (int i = 0; i < id * 500; i++)
        Delay();
    /* Exit protocol */
    printout(p3);
    egress->Wait();
    studentCount->Wait();
    (*numStudents)--;
    /* Am I the last? */
    if (*numStudents == 0)
        exited->Signal();// tell the landlord the room is empty
    printout(p4, *numStudents);
    studentCount->Signal();
    egress->Signal();        
}




