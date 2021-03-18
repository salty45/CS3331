/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                     UserID: selarkin  */
/* DUE DATE: 04/20/2020                                                     */
/* PROGRAM ASSIGNMENT #5                                                    */
/* FILE NAME: thread.cpp                                                    */
/* PROGRAM PURPOSE:                                                         */
/*    Provides implementations of threads and monitor for simulating the    */
/*    reindeer and elves and Santa program.                                 */ 
/* ------------------------------------------------------------------------ */
#include "ThreadClass.h"
#include "thread.h"

/* ------------------------------------------------------------------------ */
/* FUNCTION: initCVs                                                        */
/*      Initializes condition variables for use throughtout the program.    */
/* PARAMETER USAGE:  NONE                                                   */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::initCVs()
{
    // For reindeer
    allWaiting = new Condition("allWaiting");
    allGathered = new Condition("ag");   
    allHarnessed = new Condition("ah");
    allFlying = new Condition("af");
    allOnHoliday = new Condition("aoh");

    gathering = new Condition("gather");
    harness = new Condition("harness");
    flying = new Condition("flying");
    vacation = new Condition("vacation");

    waitReindeer = new Condition("waitReindeer");
    santaBusyr = new Condition("Santabusyr");

    wakeSanta = new Condition("wakeSanta");


    // For Elves
    answer = new Condition("answer");
    answered = new Condition("answered");
    otherElves = new Condition("others");
    turn = new Condition("turn");
    santaBusy = new Condition("santaBusy");
    questionAnswered = new Condition("qa");
    elfQ = new Condition("eq");
    santaCanAnswer = new Condition("sca");
    askSanta = new Condition("aksSanta");
    allOut = new Condition("allOut");
    elvesHere = new Condition("eHere");
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: resetReinVars                                                  */
/*      Resets variables for the reindeer to initial values                 */
/* PARAMETER USAGE:  NONE                                                   */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::resetReinVars()
{
    int i = 0;
    for (i = 0; i < reindeer; i++)
        lastBack[i] = false;

    numBack = 0;
    gathered = 0;
    harnessed = 0;
    numFlying = 0;
    onHoliday = 0;
    waitOthers = 0;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: initVars                                                       */
/*      Initializes variables to be used throughtout the program            */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED:                                                         */
/*      resetReinVars: utility sets some variables for reindeer             */
/* ------------------------------------------------------------------------ */
void NorthPole::initVars()
{
    // Called by constructor
    lastBack = (bool *) malloc(sizeof(bool) * reindeer);
    resetReinVars(); 
  
    numElfGroups = 0;
    elvesWaiting = 0;
    int i = 0;
    for (i = 0; i < 3; i++)
        elfGroup[i] = 0;
    withSanta = 0;
    numSanta = 0;
    pres = 0;
    toysDelivered = 0;
    done = false;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: NorthPole                                                      */
/*      Constructor for the monitor initializes all values and condition    */
/*      variables                                                           */
/* PARAMETER USAGE:                                                         */
/*      e: the number of elves                                              */
/*      r: the number of reindeer                                           */
/*      t: the number of toys                                               */
/* FUNCTION CALLED:                                                         */
/*      initVars:  utility initializes some variables                       */
/*      initCVs: utility initializes some condition variables               */
/* ------------------------------------------------------------------------ */
NorthPole::NorthPole(char *Name, int e, int r, int t)
            : Monitor(Name, HOARE)
{
    reindeer = r;
    elves = e;
    deliveries = t;
    initVars();
    initCVs();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: DeliveriesDone                                                 */
/*    Sets the deliveries done so nothing else prints out and prints out    */
/*    the message that Santa's on vacation.                                 */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::DeliveriesDone()
{
    MonitorBegin();
    done = true;
    printf("After (%d) deliveries, Santa retires and is on vacation!\n", 
        deliveries);
    MonitorEnd();
 
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ReindeerBack                                                   */
/*    Let Santa know the reindeer have returned                             */
/* PARAMETER USAGE:                                                         */
/*    i: id of the calling reindeer                                         */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::ReindeerBack(int i)
{
    MonitorBegin();
    if (++numBack == reindeer) {
        lastBack[i - 1] = true;
    }
    if (!done)
        printf("%4sReindeer %d returns\n", "", i);
    
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: PrintMe                                                        */
/*    Provides printing utility to threads with mutual exclusion.           */
/* PARAMETER USAGE:                                                         */
/*    format: the string to print                                           */
/*    numArgs: 0 or the negative of an integer to be printed                */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::PrintMe(char *format, int numArgs)
{
    MonitorBegin();
    if (!done) {
        if (numArgs < 0)
            printf(format, "", -1 * numArgs);
        else if (numArgs == 0)
            printf(format, "");
    }
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: WaitOthers                                                     */
/*    Reindeer wait for the others to return, with the last one waking Santa*/ 
/* PARAMETER USAGE:                                                         */
/*    i: id of the calling reindeer                                         */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::WaitOthers(int i)
{
    MonitorBegin();
    int j = 0;

    // Make sure the others are all here before continuing
    if (++waitOthers == reindeer) 
    {
        for (j = 0; j < reindeer; j++)
            if (lastBack[j])
                break;
        // Wait if Santa is with the elves
        if (santaState != SLEEP && santaState != REIN)
            santaBusyr->Wait();
        santaState = REIN;
        if (!done)
            printf("%4sThe last reindeer %d wakes up Santa\n", "", j + 1);
        
        wakeSanta->Signal();
        
        allWaiting->Signal();
    }
    else
    {
        waitReindeer->Wait();
        waitReindeer->Signal();
    }
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: WaitThenSignal                                                 */
/*    Utility for wait signal cascades                                      */
/* PARAMETER USAGE:                                                         */
/*      c: the condition varaible on which to wait and cascade              */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::WaitThenSignal(Condition *c)
{
    // only called by methods that already have a lock on the monitor
    if (done)
        return;
    c->Wait();
    c->Signal();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: WaitSleigh                                                     */
/*   Reindeer wait for Santa to harness them to the sleigh.  First they let */
/*   him know they're all gathered for harnessing; then they wait to be     */
/*   harnessed and finally let him know they're all strapped in.            */
/* PARAMETER USAGE:                                                         */
/*   i : id of calling reindeer                                             */
/* FUNCTION CALLED:                                                         */
/*      WaitThenSignal: utility for cascading wait and signal calls         */
/* ------------------------------------------------------------------------ */
void NorthPole::WaitSleigh(int i)
{
    MonitorBegin();
    // Gather by sleigh
    if (++gathered == reindeer) {
        allGathered->Signal();
    } else {
        WaitThenSignal(gathering);
    }
    
    // Harness up 
    if (++harnessed == reindeer) {
        allHarnessed->Signal();
    } else {
         WaitThenSignal(harness);
    }
    MonitorEnd();   
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: FlyOff                                                         */
/*   The reindeer fly off, deliver presents, and wait for Santa to release  */
/*   them for their holiday.                                                */
/* PARAMETER USAGE:                                                         */
/*    i : id of calling reindeer                                            */
/* FUNCTION CALLED:                                                         */
/*    WaitThenSignal: utility for cascading signals                         */
/* ------------------------------------------------------------------------ */  
void NorthPole::FlyOff(int i)
{
    bool ret = false;
    MonitorBegin();
    ret = done;
   
    // Deliver toys 
    if (++numFlying == reindeer) {
        allFlying->Signal();
    } else {
        WaitThenSignal(flying);
    }

    // Wait for holiday
    if (++onHoliday == reindeer) {
        allOnHoliday->Signal();
        if (waits != 1)
            WaitThenSignal(vacation);    
        waits = 0;   
    } else {
        WaitThenSignal(vacation);
    }
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: Group3Elves                                                    */
/*      Put three elves into a group to ask Santa a question.  Other elves  */
/*      wait until they can join a different group of three                 */
/* PARAMETER USAGE:                                                         */
/*      i: the id of the calling elf                                        */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::Group3Elves(int i)
{
    // called by method that already has a monitor lock
    int ret = 0;
    int j = 0;

    // Wait if there are more than three elves already here
    if (++pres > 3) {
        turn->Wait();
        /* Release other waiting elves if there aren't already three waiting
         * including me. */
        if (++elfs < 3 && elvesWaiting < 3) {
            turn->Signal();// See if this works
        }
    }

    // Register a group of three and wake Santa
    if (++elvesWaiting == 3) {
        numElfGroups = 1; 

        // Wait if Santa's busy with the reindeer
        if (santaState  == REIN)
            santaBusy->Wait();
        santaState = ELVES;
        elfs = 0; 
        numElfGroups = 1;
        elfGroup[elvesWaiting - 1] = i;
        if (!done) {
            printf("%9sElves %d, %d, %d wake up the Santa\n", "", elfGroup[0],
                elfGroup[1], elfGroup[2]);
        }
        wakeSanta->Signal();
        elvesHere->Signal();
    } else {
        elfGroup[elvesWaiting - 1] = i;
        otherElves->Wait();
        otherElves->Signal();
    }
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ElvesEnter                                                     */
/*      A group of elves waits for Santa to let them in to ask questions    */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::ElvesEnter()
{
    // called by a method that already has a monitor lock
    if (++asking == 3)
    {
        elfQ->Signal();
    }
    else
    {
        askSanta->Wait();
        askSanta->Signal();
    }
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ElvesExit                                                      */
/*      A group of elves gets its question answered and tells Santa when    */
/*      they exit                                                           */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::ElvesExit()
{
    // called by a method that already has a monitor lock
    int j = 0;

    //Have each of us received an answer?
    if (++withSanta == 3) {
        if (!canElfExit)
            questionAnswered->Wait();
        canElfExit = 0;
        elvesLeft = 0;
        answered->Signal();
    } else {
        answer->Wait();
        answer->Signal();
    }

    // We're leaving
    if (++elvesLeft == 1) {
        if (!done) {
        printf("%9sElves %d, %d, %d return to work\n", "", elfGroup[0], 
            elfGroup[1], elfGroup[2]);
        }
        for (j = 0; j < 3; j++)
            elfGroup[j] = 0;
    }

    // We've all left
    if (elvesLeft == 3)
        allOut->Signal();   
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: AskQuestion                                                    */
/*      Elf asks a question and gets it answered by Santa after waiting for */
/*      some amount of time.                                                */
/* PARAMETER USAGE:                                                         */
/*      i: the elf's id number                                              */
/* FUNCTION CALLED:                                                         */
/*      Group3Elves: put the elf in a group of three or wait until possible */
/*      ElvesEnter: the elf is in a group talking to Santa                  */
/*      ElvesExit: the elf is in a group which Santa has answered           */
/* ------------------------------------------------------------------------ */
void NorthPole::AskQuestion(int i)
{
    MonitorBegin();
    if (!done)
        printf("%9sElf %d has a problem\n", "", i);
    Group3Elves(i);
    ElvesEnter();
    ElvesExit();
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: AdmitElves                                                     */
/*      Admit a group of three elves and answer their questions.            */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::AdmitElves()
{
    int r = 0;
    MonitorBegin();
    canElfExit = 0; // you can't leave until I say so

    // make sure all three elves come in
    if (elvesWaiting != 3)
        elvesHere->Wait();
    elvesWaiting = 0;
    otherElves->Signal();

    // make sure they're all here before they can ask their questions
    if (asking != 3)
        elfQ->Wait();
    asking = 0;
    askSanta->Signal();   
    
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ReleaseElves                                                   */
/*      Release the elves after answering their questions, waiting to make  */
/*      sure all the elves have really left.                                */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::ReleaseElves()
{
    MonitorBegin();
    if (!done) {
        printf("Santa answers the question posted by elves %d, %d, %d\n",
            elfGroup[0], elfGroup[1], elfGroup[2]);
    }
    canElfExit = 1;
    questionAnswered->Signal();
    
    // Make sure everyone's really here and release them
    if (withSanta != 3)
        answered->Wait();
    withSanta = 0;
    answer->Signal();

    // Wait for them all to leave and then clean up the variables
    if (elvesLeft != 3)
    {
        allOut->Wait();
    }
    elvesLeft = 0; 
    pres = pres - 3;
    santaState = SLEEP;
    numElfGroups -= 1; 
    turn->Signal();
    MonitorEnd();
}
 
/* ------------------------------------------------------------------------ */
/* FUNCTION: ElfGroups                                                      */
/*     Return the number of elf groups waiting                              */
/* PARAMETER USGAE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
bool NorthPole::ElfGroups()
{
    bool ret = false;
    MonitorBegin();
    ret = numElfGroups > 0;
    MonitorEnd();
    return ret;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: Sleep                                                          */
/*      Santa sleeps until woken by presence of reindeer or elves.          */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
int NorthPole::Sleep()
{
    int ret = 0;
    
    MonitorBegin();
    santaState = SLEEP;// I'm asleep
   
    /* If there's nobody here, I'll wait and snooze */
    if (numBack != reindeer && numElfGroups != 1)
        wakeSanta->Wait();

    // Who woke me - I need to signal them if they're waiting for me
    if (numBack == reindeer) {
        santaState = REIN;
        santaBusyr->Signal();      
    } else {
        santaState = ELVES;
        santaBusy->Signal();
    }
    MonitorEnd();    
    return ret;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: GatherReindeer                                                 */
/*      Santa gathers the reindeer, harnesses them, and sends them out for  */
/*      delivering the toys. Then prints a message and updates the number   */
/*      of toys delivered stored by the monitor.                            */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
void NorthPole::GatherReindeer()
{
    int i = 0;
    MonitorBegin();

    // Are all the reindeer back?
    if (waitOthers != reindeer)
        allWaiting->Wait();
    waitOthers = 0;
    waitReindeer->Signal();
    // Gather them for harnessing
    if (gathered != reindeer)
        allGathered->Wait();
   
    if (!done) 
        printf("Santa is preparing sleighs\n");
    gathering->Signal();

    // Harness them
    if (harnessed != reindeer)
        allHarnessed->Wait();
    harness->Signal();

    // Go deliver those presents!
    if (numFlying != reindeer)
        allFlying->Wait();
    flying->Signal();
    printf("The team flies off (%d)!\n", ++toysDelivered);
    MonitorEnd();   
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ReleaseReindeer                                                */
/*      Releases the reindeer who are waiting to go on holiday, updates the */
/*      number of toy deliveries, and signals that Santa is now free        */
/* PARAMETER USAGE:                                                         */
/*      n: pointer to Santa's variable to track number of toy deliveries    */
/* FUNCTION CALLED:                                                         */
/*      resetReinVars: reset variables associated with the reindeer         */
/* ------------------------------------------------------------------------ */ 
void NorthPole::ReleaseReindeer(int *n)
{
    int i = 0;
    MonitorBegin(); 
    
    // Wait for reindeer to return
    if (onHoliday != reindeer)
        allOnHoliday->Wait();
    
    // update toys delivered
    *n = toysDelivered;
    // update variables and signal that the cycle is finished
    resetReinVars();
    waits = 1;
    vacation->Signal();
    santaState = SLEEP;
    santaBusy->Signal();    
    MonitorEnd();
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: WhoWokeMe                                                      */
/*    Return r if reindeer woke Santa, e if elves woke santa, 0 otherwise.  */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
char NorthPole::WhoWokeMe() {
    char ret = 0;
    MonitorBegin();
    if (santaState == REIN)
        ret = 'r';
    else if (santaState == ELVES)
        ret = 'e';
    MonitorEnd();
    return ret;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: Santa                                                          */
/*     Constructor for santa thread                                         */
/* PARAMETER USAGE:                                                         */
/*     td: number of toy deliveries to perform                              */
/*     np: the monitor                                                      */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
Santa::Santa(NorthPole *np, int td)
{
    liz = np;
    toyDeliveries = td;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ThreadFunc                                                     */
/*     Performs the loop for Santa. I changed to a while loop rather than a */
/*     for-loop to make the logic update a bit easier for the end.          */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED:                                                         */
/*     PrintMe: printing utility                                            */
/*     WhoWokeMe: determine who woke Santa                                  */
/*     GatherReindeer: gather the reindeer and deliver toys                 */
/*     ReleaseReindeer: release the reindeer and increment the number of    */
/*                       toy deliveries.                                    */
/*     ElfGroups: see if there are any elf groups with questions            */
/*     AdmitElves: admit a group and listen to questions                    */
/*     ReleaseElves: answer the elves' questions and send them back to work */
/*     DeliveriesDone: tell everyone I'm retiring                           */
/* ------------------------------------------------------------------------ */
void Santa::ThreadFunc()
{
    int i, reason;
    Thread::ThreadFunc();
    liz->PrintMe("Santa thread starts\n", 0);
    while (i < toyDeliveries)
    {
        reason = liz->Sleep();
        // look after reindeer first
        if (liz->WhoWokeMe() == 'r')
        {
            liz->GatherReindeer();
            Delay();
            liz->ReleaseReindeer(&i);
        }
        
        // then look after elves
        else if (liz->ElfGroups())
        {
            liz->AdmitElves();
            
            liz->PrintMe("Santa is helping elves\n", 0);
            Delay();
            liz->ReleaseElves();
        }
    }
    liz->DeliveriesDone();
    exit(0);
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: Elf                                                            */
/*     Constructor for elf thread                                           */
/* PARAMETER USAGE:                                                         */
/*     i: the reindeer's id number                                          */
/*     np: the monitor                                                      */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
Elf::Elf(int i, NorthPole *np)
{
    liz = np;
    id = i;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ThreadFunc                                                     */
/*     Performs the loop for the elf.                                       */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED:                                                         */
/*     PrintMe: printing utility                                            */
/*     AskQuestion: ask Santa my question                                   */
/* ------------------------------------------------------------------------ */
void Elf::ThreadFunc()
{
    Thread::ThreadFunc();
    liz->PrintMe("%9sElf %d starts\n", -1 * id); 
    while(true)
    {
        Delay();
        liz->AskQuestion(id);
        Delay();
    }
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: Reindeer                                                       */
/*     Constructor for reindeer thread                                      */
/* PARAMETER USAGE:                                                         */
/*     i: the reindeer's id number                                          */
/*     np: the monitor                                                      */
/* FUNCTION CALLED: NONE                                                    */
/* ------------------------------------------------------------------------ */
Reindeer::Reindeer(int i, NorthPole *np)
{
    id = i;
    liz = np;
}

/* ------------------------------------------------------------------------ */
/* FUNCTION: ThreadFunc                                                     */
/*     Performs the loop for the reindeer.                                  */
/* PARAMETER USAGE: NONE                                                    */
/* FUNCTION CALLED:                                                         */
/*     PrintMe: printing utility                                            */
/*     ReindeerBack: notify Santa that I'm back                             */
/*     WaitOthers: wait for the other reindeer                              */
/*     WaitSleigh: wait to be harnessed                                     */
/*     FlyOff: deliver presents and wait for Santa to release me            */
/* ------------------------------------------------------------------------ */
void Reindeer::ThreadFunc()
{
    Thread::ThreadFunc();
    liz->PrintMe("%4sReindeer %d starts\n", -1 * id);
    while(true)
    {
        Delay();
        liz->ReindeerBack(id);
        liz->WaitOthers(id);
        liz->WaitSleigh(id);
        liz->FlyOff(id);
        Delay();
    }
}
