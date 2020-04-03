#include "thread.h"

void Landlord::kickStudentsOut()
{
    //int studentRm = numStudents;
    printout(p4, *numStudents);
    printout(p5, 0);
    studentCount->Signal();
    egress->Signal();
    exited->Wait(); 
    printout(p6, 0);
}

void Landlord::retire()
{
    bahamas->Wait();
    *landlordRetired = true;
    bahamas->Signal();
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
    int j = 0;
    /* Entry protocol */
    door->Wait();
    //printf("in entry prot\n");
    //fflush(stdout);
    *landlordPresent = true;
    //printf("lp\n");
    //fflush(stdout);
    egress->Wait();
    //printf("entry wait\n");
    entry->Wait();
    //printf("Eg wait\n");
    //fflush(stdout);
    printout(p1, i + 1);
    //door->Signal();

    /* In CS */
    //printout(p1, i + 1);
    printout(p1, (*numStudents));
    Delay();

    /* Exit protocol */
    //printf("waiting door \n");
    //door->Wait();
    //printf("waiting sc\n");
    studentCount->Wait();
    //printf("have sc\n");
   
    /* Is it time to retire? */
    if (i == m - 1)
    {
        retire();
        kickStudentsOut();
        //printf("sunny beaches ... ahh!\n");
        entry->Signal();
        door->Signal();
        for (j = 0; j < totalStudents; j++)
        {
         //   printf("land w: %d %d\n", j, totalStudents);
            allDone->Wait();
        }
        printout(p7, m);
        Exit();
    }
    *landlordPresent = false;
    if (*numStudents == 0)
    {
        printout(p2, *numStudents);
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
        // kick 'em out
    }
    //printf("door sig\n");
    //fflush(stdout);
    door->Signal();
    //printf("ent sig\n");
    //fflush(stdout);
    //for (j = 0; j < numWaiting; j++)
        entry->Signal();   
}
    /* Printout message 2 */
 /*   if (*numStudents <= n)
    {
        printf("%d\n", *numStudents);
        if (*numStudents == 0)
            printout(p2, *numStudents);
        else
            printout(p3, *numStudents);

        if (i < m - 1)
            egress->Signal();
    }
    else if (*numStudents > n) 
    {
        printout(p4, *numStudents);
        kickStudentsOut();
    }  */        

    /* Time to retire to a warm climate! */
/*    if (i == m - 1)
    {
        if (*numStudents > 0)
            kickStudentsOut();
        else
            studentCount->Signal();// for completeness of freeing sems
        retire();// retire 1st so any entering student gets an eviction
        printf("retirement party!\n");
        entry->Signal();// notice and doesn't come in
        for (int i = 0; i < totalStudents; i++)
            allDone->Wait();// wait for all student threads to terminate
        printout(p7, m);// then print out final message
        Exit();
    }*/
   /* *landlordPresent = false;
    studentCount->Signal();
    printf("yield sc\n");
    //*landlordPresent = false;
    printf("lp: %d\n", *landlordPresent);
    entry->Signal();*/
   // door->Signal();   


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
 //   printf("%s: waiting bah\n", msg);
    bahamas->Wait();
   // printf("%s: have bah\n", msg);
    if (*landlordRetired)
    {
        bahamas->Signal();
        door->Signal();
        studentQueue->Signal();
        printout(p5);
        allDone->Signal();
        Exit();
    }
    //printf("%s wait sc\n", msg);
    studentCount->Wait();
    //printf("%s have sc\n", msg);
    (*numStudents)++;
   // printf("%s sns: %d\n", msg, *numStudents); 
    /* Wait on exited if I'm the first one here.  This makes it so the landlord
     * can tell when the last one has left. */
    if (*numStudents == 1)
    {
    //    printf("%s before exited\n", msg);
        exited->Wait();
      //  printf("%s past exited\n", msg);
    }
    printout(p2, *numStudents);
    studentCount->Signal();
   // door->Signal();
    bahamas->Signal();
    
}

void Student::GoToParty()
{
    printout(p1); 
    studentQueue->Wait();
    door->Wait();
 //   printf("%s %d\n", msg, landlordPresent);   
    /* Yield if the landlord was at the door first */
    if (*landlordPresent)
    {
   //     printf("%s lp\n", msg);
        *StudentsWaiting++;// probably not needed here!
        door->Signal();// release the door for the landlord
        studentQueue->Signal();// release the queue entry for students
        entry->Wait();// wait for the landlord to leave
        entry->Signal();
        door->Wait();
        
        enterRoom();
        door->Signal();
        //entry->Signal();
    }
    else
    {
        enterRoom();
        door->Signal();
        studentQueue->Signal();
    }
   
    /* In CS */
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




