#include "thread.h"

void Landlord::kickStudentsOut()
{
    //int studentRm = numStudents;
    printout(p5, 0);
    studentCount->Signal();
    exited->Wait(); 
    printout(p6, 0);
}

void Landlord::retire()
{
    wait(bahamas);
    landlordRetired = true;
    signal(bahamas);
}


void Landlord::printout(char *format, int n)
{
    char *m2;
    sout->Wait();
    if (n <= 0)
        printf(format, "");
    else
        printf(format, n);
    sout->Signal();
}

void Landlord::CheckRoom(int i) 
{
    /* Entry protocol */
    door->Wait();
    landlordPresent = true;
    egress->Wait();
    door->Signal();

    /* In CS */
    printout(p1, i + 1);
    Delay();

    /* Exit protocol */
    studentCount->Wait();

    /* Printout message 2 */
    if (numStudents == 0)
    {
        printout(p2, numStudents);
    }
    else if (numStudents <= n)
    {
        printout(p3, numStudents);
    }
    else if (numStudents > n) 
    {
        printout(p4, numStudents);
        kickStudentsOut();
    }          

    /* Time to retire to a warm climate! */
    if (i == m - 1)
    {
        if (numStudents > 0)
            kickStudentsOut();
        else
            studentCount->Signal();// for completeness of freeing sems
        retire();// retire 1st so any entering student gets an eviction
        entry->Signal();// notice and doesn't come in
        allDone->Wait();// wait for all student threads to terminate
        printout(p7, m);// then print out final message
        Exit();
    }

    entry->Signal();   
}

/*void Landlord::CheckRoom()
{
     Entry Protocol 
    LandlordNext->Wait();
    landlordPresent = true;
    LandLordNext->Signal();
    DoorLock->Wait();
    Enter->Wait();
    Exit->Wait();
    DoorLock->Signal();

    
    Delay();    

     Exit Protocol 
    wait(studentCount);
    if (i == m - 1)
    {
        tellStudentsToLeave();
    }
    else if (numStudents > n)
    {
        tellStudentsToLeave();
    } 
    else
    {
        Exit->Signal();
    }

    landLordPresent = false;
    
  let waiting students know they can enter 
    for (j = 0; j < waitingEntry; j++)
        Entry->Signal();

    
}

Student::GoToParty()
{
    DoorLock->Wait();
    VarMutex->Wait();
    if (landLordPresent)
    {
        waitingEntry++;
        VarMutex->Signal();
        DoorLock->Signal();
        Entry->Wait();
    }
            



    }

}*/


Student::printout(char *format, int n)
{
    sout->Wait();
    if (n < 0)
        printf(format, msg);
    else
        printf(format, msg, n);
    sout->Signal();
}


Student::EnterRoom()
{
    bahamas->Wait();
    if (landlordRetired)
    {
        bahamas->Signal();
        Exit();
    }
    studentCount->Wait();
    numStudents++;
    
    /* Wait on exited if I'm the first one here.  This makes it so the landlord
     * can tell when the last one has left. */
    if (numStudents == 1)
        exited->Wait();

    studentCount->Signal();
    bahamas->Signal();
}

Student::GoToParty()
{
    
    studentQueue->Wait();
    door->Wait();
    
    /* Yield if the landlord was at the door first */
    if (landlordPresent)
    {
        studentsWaiting++;// probably not needed here!
        door->Signal();// release the door for the landlord
        studentQueue->Signal();// release the queue entry for students
        entry->Wait();// wait for the landlord to leave
        enterRoom();
    }
    enterRoom();
    door->Signal();
    studentQueue->Signal();
   
    /* In CS */

    /* Exit protocol */
    exit->Wait();
    studentCount->Wait();
    numStudents--;
    /* Am I the last? */
    if (numStudents == 0)
        exited->Signal();// tell the landlord the room is empty
    studentCount->Signal();
    exit->Wait();        
}




