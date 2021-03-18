////
//Student::EnterRoom()
//{
//    if(lr)
//    {
//        r->print("s-term");
//        Done->Signal();
//        Exit();
//    }
//
//    numStudents++;
//    if (numStudents == 1)
//        Exited->Wait();
//    Door->Signal();
//    Queue->Signal();
//}
//
//
//
//Student::GoToParty()
//{
//    Queue->Wait();
//    Door->Wait();
//    if (lp)
//    {
//        stuw++;
//        Door->Signal();
//        //Queue->Signal();
//     
//        // baton pass - I now have access to the entry stuff!
//        Enter->Wait();
//        Door->Wait();
////        if (lr)
////        {
//            //print term
////            Entered->Signal();// enter->Signal?
////            Done->Signal();
////            Exit();
////        }
//        numStudents++;
//        if (numStudents == 1)
//            Exited->Wait();
//        stuw--;
//        if (stuw != 0)
//            Enter->Signal();
//        Door->Signal();
//        Queue->Signal();//???
//        //Enter->Signal();
//    }
//    else
//    {
//        numStudents++;
//        if (numStudents == 1)
//            Exited->Wait(); // keep this one at zero for landlord
//        Door->Signal();
//        Queue->Signal();
//
//    }
//
//    // In CS
//    
//
//    Door->Wait();
//  //  Exit->Wait();
//    
//    // Get kicked out
//    if (lp)
//    {
//        Door->Signal();
//        Exit->Wait();
//        numStudents--;
//        if (numStudents == 0)
//            Exited->Signal();// room's empty
//        Exit->Signal();// let the next one go!
//    }
//    else
//    {
//        // leave because I want to
//        numStudents--;
////        Exit->Signal();
//        Door->Signal();
//    }
//}
//


#include "thread.h"
#include <stdio.h>
#include <stdlib.h>


Room::Room(int k)
{

    stuw = 0;
    numStudents = 0;
    lp = 0;
    lr = 0;
    
    Door = new Semaphore("d", 1);
    Queue = new Semaphore("q", 1);// may need to be more
    Enter = new Semaphore("e", 0);
    Exit = new Semaphore("x", 0);
    Exited = new Semaphore("xd", 1);
    Done = new Semaphore("on", 0);
    Sout = new Semaphore("y", 1);
    Entered = new Semaphore("ed", 0);
}



Student::Student(int sid, Room *room)
{
    r = room;
    id = sid;
    sprintf(msg, smsg, "", id);
    //r->m = msg; 
}


void Student::ThreadFunc() {
    Thread::ThreadFunc();
    r->print(s0, -1, id);

    /* This student sure parties a lot! */
    while (1)
    {
        Delay();
        GoToParty();
        Delay();
    }
}

Landlord::Landlord(int m, int n, int k, Room *room)
{
    this->m = m;
    this->n = n;
    this->k = k;

    r = room;
    
}


void Landlord::ThreadFunc()
{
    Thread::ThreadFunc();
    int i = 0;
    r->print(p0, -1, 0);

    /* Landlord checks the room m times before retiring */
    for (i = 1; i <= m; i++)
    {
        Delay();
        CheckRoom(i);
        Delay();
    }
}
























      
