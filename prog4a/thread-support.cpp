#include "thread.h"


void Room::print(char *msg, int n, int a)
{
    //sprintf(m, smsg, "", a);
    Sout->Wait();
    sprintf(m, smsg, "", a);
    if (n < 0)
    {
        if (!a)
            printf(msg, "");
        else
            printf(msg, m);
    }
    else
    {
        if (!a)
           printf(msg, n);
        else
           printf(msg, m, n);
    }
    Sout->Signal();
}


void Student::EnterRoom()
{
    if(r->lr)
    {
        r->print(s5, 0, id);
        r->Done->Signal();
        r->Door->Signal();
        r->Queue->Signal();
        r->Entered->Signal();
        Exit();
    }

    r->print("%s xd wait %d\n", r->numStudents, id);
    //if (++(r->numStudents) == 1)
      //  r->Exited->Wait();
    r->numStudents++; 
    r->print(s2, r->numStudents, id);
    r->Door->Signal();
    r->Queue->Signal();
}


void Student::GoToParty()
{
    r->print(s1, -10, id);
    r->Queue->Wait();
    r->print("%shas queue: \n", -1, id);
    r->Door->Wait();
    r->print("%s has door\n", -1, id);
    if (r->lp)
    {
        r->stuw++;
        r->Door->Signal();
        
        r->print("%s w for enter\n", -1, id);
        r->Enter->Wait();
        r->print("%s had enter\n", -1, id);
        r->Door->Wait();
        r->print("%s door2\n", -1, id);
        r->stuw--;
        if (r->stuw != 0)
            r->Enter->Signal();
        else
            r->Entered->Signal();
        EnterRoom();
    }
    else
    {
        EnterRoom();
    }

    // In CS
    for (int j = 0; j < id * 500; j++)
        Delay();

    r->print(s3, 0, id);
    r->Door->Wait();
    if (r->lp)
    {
        r->Door->Signal();
        r->Exit->Wait();
     
        r->numStudents--;     
        r->print(s4, r->numStudents, id);     
        if ((r->numStudents) == 0)
            r->Exited->Signal();
        else
            r->Exit->Signal();
    }
    else
    {   
        r->numStudents--;
        r->print(s4, r->numStudents, id);
 //       if (r->numStudents == 0)
   //         r->Exited->Signal();// Do I need this here?
        r->Door->Signal();
    }
}


void Landlord::CheckRoom(int i)
{
    int j = 0;
    r->Door->Wait();
    //r->Enter->Wait(); 
    //Exit->Wait();
    r->lp = 1;
    r->print(p1, i, 0);
    r->Door->Signal();


    // In CS    
    Delay();

    r->Door->Wait();
    if (i == m)
    {
        r->lr = 1;
        kickeroo();
        allowIn();//removes the landlord's presence
        r->print("ll allowed\n", -10, 0); 
        // Wait for everyone else to terminate
        for (j = 0; j < k; j++)
            r->Done->Wait();
        // print and exit;
        r->print(p7, m, 0); 
        Exit();
    }

    if (r->numStudents <=n)
    {
        r->print("ns: %d\n", r->numStudents, 0);
        if (r->numStudents == 0)
            r->print(p2, -1, 0);
        else
            r->print(p3, r->numStudents, 0);
        r->lp = 0;
        r->Door->Signal();
    }

    if (r->numStudents > n)
    {
        kickeroo();
        allowIn();     
    }
}

void Landlord::kickeroo()
{
    // may be able to remove j and use just num students
    //int j = numStuents;
    r->Door->Signal();
    //while (j > 0)
    //{
        r->print(p4, r->numStudents, 0);
        r->print(p5, -1, 0);
        r->Exit->Signal();// start cascading release
        r->print("exittime\n", -1, 0);
        r->Exited->Wait();// wait for the last to be released
        r->print(p6, -1, 0);
      //  j--;
    //}
}

void Landlord::allowIn()
{
    r->print("allow \n", -10, 0);
    // At this point only the landlord will really be able to enter.
    r->Door->Wait();
    r->lp = 0; // I'm leaving
    r->Enter->Signal();// you can try to come on in + cascade
    r->Door->Signal();// done with my modifications
    r->Entered->Wait();// wait for y'all to enter again
}
