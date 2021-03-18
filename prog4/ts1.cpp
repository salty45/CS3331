#include "thread.h"

void Landlord::kickStudentsOut()
{
    //int studentRm = numStudents;
    printout(p4, *numStudents);
    printout(p5, -1);
    studentCount->Signal();
    egress->Signal();
    exited->Wait(); 
    printout(p6, -1);
}

void Landlord::retire()
{
    bahamas->Wait();
    *landlordRetired = true;
    bahamas->Signal();
}


void Landlord::printout(char *format, int n)
{
    sout->Wait();
    if (n < 0)
        printf(format, "");
    else
        printf(format, n);
    sout->Signal();
}

void Landlord::CheckRoom(int i) 
{
    int j = 0;
    /* Entry protocol */
    door->Wait();
    *landlordPresent = true;
    egress->Wait();
    entry->Wait();
    printout(p1, i + 1);

    /* In CS */
    Delay();

    /* Exit protocol */
    studentCount->Wait();
    
    /* Is it time to retire? */
    if (i == m - 1)
    {
        retire();
        kickStudentsOut();
        entry->Signal();
        door->Signal();
        
        /* Wait for all my students to leave */
        for (j = 0; j < totalStudents; j++)
              allDone->Wait();
        printout(p7, m);
        Exit();
    }
    *landlordPresent = false;
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
    entry->Signal();   
}
   
void Student::printout(char *format, int n)
{
    sout->Wait();
    if (n < 0)
        printf(format, msg);
    else
        printf(format, msg, n);
    sout->Signal();
}

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

void Student::GoToParty()
{
    printout(p1); 
    studentQueue->Wait();
    door->Wait();
    
    /* Yield if the landlord was at the door first */
    if (*landlordPresent)
    {
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
    
    for (int i = 0; i < id; i++)
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




