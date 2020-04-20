/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                     UserID: selarkin  */
/* DUE DATE: 04/17/2020                                                     */
/* PROGRAM ASSIGNMENT #5                                                    */
/* FILE NAME: thread.cpp                                                    */
/* PROGRAM PURPOSE:                                                         */
/*    Provides implementations of threads and monitor for simulating the    */
/*    reindeer and elves and Santa program.                                 */ 
/* ------------------------------------------------------------------------ */
#include "ThreadClass.h"
#include "thread.h"
/**
 * FUNCTION: initCVs
 *      Initializes condition variables for use throughtout the program.
 * Parameter usage: none
 * Function Called: none
 */
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

    wakeSanta = new Condition("wakeSanta");


    // For Elves
    answer = new Condition("answer");
    answered = new Condition("answered");
    otherElves = new Condition("others");
    turn = new Condition("turn");
    admit = new Condition("admit");
    elfGroupHere = new Condition("egh");
    santaBusy = new Condition("santaBusy");
    questionAnswered = new Condition("qa");
    elfQ = new Condition("eq");
    santaCanAnswer = new Condition("sca");
    askSanta = new Condition("aksSanta");
    allOut = new Condition("allOut");
    santaBusyr = new Condition("Santabusyr");
    elfDone = new Condition("elfdone");
    elvesHere = new Condition("eHere");
}

/*
 * FUNCTION: resetReinVars
 *      Resets variables for the reindeer
 * PARAMETER USAGE:  NONE
 * FUNCTION CALLED: NONE
 */
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

/*
 * FUNCTION: initVars
 *      Initializes variables to be used throughtout the program
 * PARAMETER USAGE: NONE
 * FUNCTION CALLED: 
 *  resetReinVars: utility sets some variables for reindeer
 */
void NorthPole::initVars()
{
    // For reindeer
    lastBack = (bool *) malloc(sizeof(bool) * reindeer);
    resetReinVars();   
    santaSleeping = false;
    
    reindeerDone = 0;
    elvesDone = 0;

    elvesWaiting = 0;
    int i = 0;
    for (i = 0; i < 3; i++)
        elfGroup[i] = 0;
    numGroups = 0;
    withSanta = 0;
    numSanta = 0;
  
    santaOpen = false;
    pres = 0;
}

/*
 * FUNCTION: NorthPole
 *      Constructor for the monitor initializes all its values and condition
 *      variables
 * PARAMETER USAGE:
 *      e: the number of elves
 *      r: the number of reindeer
 *      t: the number of toys
 * FUNCTION CALLED:
 *      initVars:  utility initializes some variables
 *      initCVs: utility initializes some condition variables
 */
NorthPole::NorthPole(char *Name, int e, int r, int t)
            : Monitor(Name, HOARE)
{
//    printf("making np\n");
    int i = 0;
    char s[5];
    elves = 7;
    haveQ = 0;
    forSanta = 0;
    numElfGroups = 0;
    
    santaRetired = false;   
    for (i = 0; i < 3; i++)
        elf[i] = 0;    

    everyoneExit = new Condition("everyoneOut");
    
    reindeer = r;

    elves = e;
    deliveries = t;
    toysDelivered = 0;
    done = false;   

    initVars();
    initCVs();
}

/*
 * Function: DeliveriesDone
 *    Sets the deliveries done so nothing else prints out and prints out the
 *    message that Santa's on vacation.
 * Parameters: none
 * Functions called: none
 */
void NorthPole::DeliveriesDone()
{
    MonitorBegin();
    done = true;
    printf("After (%d) deliveries, Santa retires and is on vacation!\n", 
        deliveries);
    MonitorEnd();
 
}

/*
 * Function: ReindeerBack
 *    Let Santa know the reindeer have returned
 * Parameters:
 *    i: id of the calling reindeer
 * Function called: none
 */
bool NorthPole::ReindeerBack(int i)
{
    bool ret = false;
    MonitorBegin();
    ret = done;
    if (++numBack == reindeer) {
        lastBack[i - 1] = true;
    }
    if (!done)
        printf("%5sReindeer %d returns\n", "", i);
    
    MonitorEnd();
    return ret;
}

/*
 * Function: PrintMe
 *    Provides printing utility to threads with mutual exclusion.
 * Parameter:
 *    format: the string to print
 *    numArgs: 0 or the negative of an integer to be printed
 *    args: not really used, but could be used to print after done
 */
void NorthPole::PrintMe(char *format, int numArgs, int *args)
{
    MonitorBegin();
    if (args != NULL || !done) {
        if (numArgs < 0)
            printf(format, "", -1 * numArgs);
        else if (numArgs == 0)
            printf(format, "");
    }
    MonitorEnd();
}

/*
 * Function: WaitOthers
 *   reindeer wait for the others to return so they can gather for harnessing
 * Parameter:
 *    i: id of the calling reindeer
 * Function called: none
 */
bool NorthPole::WaitOthers(int i)
{
    bool ret = false;
    MonitorBegin();
    int j = 0;
    //printf("reindw: %d\n", i);
    if (++waitOthers == reindeer) 
// guaranteed others are all waiting = no worried about exec order
    {
        //waitOthers = 0;
        for (j = 0; j < reindeer; j++)
            if (lastBack[j])
                break;
        //printf("elfP: %d\n", elfP);
        //if (elfP)
          //  elfDone->Wait();
        //elfP = 0;
        //santaOpen = 0; 
        if (santaState != SLEEP && santaState != REIN)
            santaBusyr->Wait();
        santaState = REIN;
        if (!done)
            printf("The last reindeer %d wakes up Santa\n", j + 1);
        
        wakeSanta->Signal();
        
        allWaiting->Signal();
    }
    else
    {
     //   printf("wr: %d %d %d\n", i, numBack, waitOthers);
        waitReindeer->Wait();
        waitReindeer->Signal();
    }
    ret = done;
    MonitorEnd();
    return ret;
}

/*
 * Function: WaitThenSignal
 *    Utility for wait signal cascades
 * Parameter:
 *      c: the condition varaible on which to wait and cascade
 * Function called: none
 */
void NorthPole::WaitThenSignal(Condition *c)
{
    // only called by methods that already have a lock on the monitor
    if (done)
        return;
    c->Wait();
    c->Signal();
}

/*
 * Function: WaitSleigh
 *   Reindeer wait for Santa to harness them to the sleigh.  First they let
 *   him know they're all gathered for harnessing; then they wait to be
 *   harnessed and finally let him know they're all strapped in.
 * Parameters:
 *   i : id of calling reindeer
 * Function called:
 *      WaitThenSignal: utility for cascading wait and signal calls
 */
bool NorthPole::WaitSleigh(int i)
{
    bool ret = false;
    MonitorBegin();
    //printf("gathering %d %d\n", i, gathered);
    if (++gathered == reindeer) {
        santaOpen = 0;
        allGathered->Signal();
     } else
        WaitThenSignal(gathering);

     
    if (++harnessed == reindeer) {
        allHarnessed->Signal();
    } else {
         WaitThenSignal(harness);
    }
     
    if (harnessed == reindeer) 
 
    ret = done;
    MonitorEnd();
    return ret;   
}

/*
 * Function: FlyOff
 *   The reindeer fly off, deliver presents, and wait for santa to release them
 *   for their holiday.
 * Parameter:
 *  i : id of calling reindeer
 * Function called:
 *  WaitThenSignal: utility for cascading signals
 */
bool NorthPole::FlyOff(int i)
{
    bool ret = false;
    MonitorBegin();
    ret = done;
    
    if (++numFlying == reindeer) {
        allFlying->Signal();
    } else {
        //printf("Wanna fly! %d\n", i);
        WaitThenSignal(flying);
    }
    ret = done;
 //   printf("rih: %d\n", i);
    if (++onHoliday == reindeer)
        allOnHoliday->Signal();
    else
        WaitThenSignal(vacation);
    MonitorEnd();
    return ret;
}


/*
 * Function: Group3Elves
 *   Groups elves into their bunches of 3
 */
bool NorthPole::Group3Elves(int i)
{
    // called by method that already has a monitor lock
    int ret = 0;
    int j = 0;
    //int num3 = 0;
    if (++pres > 3) {
        printf("eld: %d %d \n", i, pres);
        turn->Wait();
        printf("Elf %d released:  waiting = %d pres = %d rel = %d\n", i, 
            elvesWaiting, pres, elfs);
        if (++elfs < 3 && elvesWaiting < 3) {
            printf("es: %d  ew: %d e: %d\n", elfs, elvesWaiting, i);
            turn->Signal();// See if this works
        }
    }
    printf("%4sE = %d ew: %d\n", "", i, elvesWaiting);
    if (++elvesWaiting == 3) {
        printf("Elf %d\n", i);
        numElfGroups = 1; 
        if (santaState  == REIN)//!= SLEEP && santaState != ELVES)
            santaBusy->Wait();
        santaState = ELVES;
        printf("elves next\n");
        elfs = 0; 
        numElfGroups = 1;
        elf3 = i;
        santaOpen = 0;
        elfGroup[elvesWaiting - 1] = i;
        printf("wake santa %d %d\n", i, elvesWaiting); 
        if (!done) {
            //santaState = ELVES;
            printf("%10sElves %d, %d, %d wake up the Santa\n", "", elfGroup[0],
            elfGroup[1], elfGroup[2]);
        }
        elfP = true;
        wakeSanta->Signal();
        elvesHere->Signal();
    } else {
        //printf("Elf %d is w %d\n", i, elvesWaiting);
        elfGroup[elvesWaiting - 1] = i;
        otherElves->Wait();
        otherElves->Signal();
    }
    return ret;
}

bool NorthPole::ElvesEnter()
{
    printf("elf ei %d\n", asking);
    // called by a method that already has a monitor lock
    if (++asking == 3)
    {
        //if (!santaElves)
          //  santaCanAnswer->Wait();
        //santaElves = 0;
        //elvesAsking = 1;
        //asking = 0;
        printf("next step\n");
        elfQ->Signal();
    }
    else
    {
        printf("wait todoor\n");
        askSanta->Wait();
        askSanta->Signal();
    }
    printf("elves in\n");
    return done;
}

bool NorthPole::AskQuestion(int i)
{
    bool ret = false;
    int j = 0;
    MonitorBegin();
    if (!done)
        printf("%10sElf %d has a problem\n", "", i);
    Group3Elves(i);

    printf("ask: %d %d %d\n", elfGroup[0], elfGroup[1], elfGroup[2]);
    ElvesEnter();

    // Elves Exit
    printf("%d: with santa: %d\n", i, withSanta);
    if (++withSanta == 3) {
        printf("Elf %d waiting to go\n", i);
        if (!canElfExit)
            questionAnswered->Wait();
        canElfExit = 0;
        elfLeaving = 1;
        //withSanta = 0;
        elvesLeft = 0;
        printf("leaving lef\n");
        answered->Signal();
    } else {
        printf("wanna leave %d ws: %d\n", i, withSanta);
        answer->Wait();
        printf("outa heere! %d\n", i);
        answer->Signal();
    }
    if (++elvesLeft == 1) {
        if (!done) {
        printf("%10sElves %d, %d, %d return to work\n", "", elfGroup[0], 
            elfGroup[1], elfGroup[2]);
   //     allOut->Signal();  
        }
        for (j = 0; j < 3; j++)
            elfGroup[j] = 0;
        //allOut->Signal();
    }
    if (elvesLeft == 3)
        allOut->Signal();   
    printf("elf x mon\n");
    MonitorEnd();
    return ret;
}


void NorthPole::AdmitElves()
{
    MonitorBegin();
 //   printf("Admitting elves\n");
   // santaElves = 1;
    canElfExit = 0;
   // santaOpen = 0;
    if (elvesWaiting != 3)
        elvesHere->Wait();
    elvesWaiting = 0;
    otherElves->Signal();
    //santaCanAnswer->Signal();
    if (asking != 3)
        elfQ->Wait();
    asking = 0;
    askSanta->Signal();   
    MonitorEnd();
}


void NorthPole::ReleaseElves()
{
    MonitorBegin();
    //printf("about to release elves\n");
    if (!done) {
        printf("Santa answers the question posted by elves %d, %d, %d\n",
        elfGroup[0], elfGroup[1], elfGroup[2]);
    }
    canElfExit = 1;
    questionAnswered->Signal();
    printf("all elves pres - go to shop!\n");
    if (withSanta != 3)
        answered->Wait();
    withSanta = 0;
    printf("Shoo already\n");
    elfLeaving = 0;
    //elvesLeft = 0;
    answer->Signal();
    printf("closing the door to snooze\n");
    if (elvesLeft != 3)
    {
        allOut->Wait();
    }
    printf("zzzz %d\n", elvesLeft);
    elvesLeft = 0; 
    pres = pres - 3;
    //santaOpen = 1;
    //?elfLeaving = 0; 
    //withSanta = 0;
    elfP = false;
    santaState = SLEEP;
    numElfGroups -= 1; 
    elvesWaiting = 0;
    elfDone->Signal();
    turn->Signal();
  //  elfDone->Signal();
    printf("santa x mon\n");
    MonitorEnd();
}
 

bool NorthPole::ElfGroups()
{
    return numElfGroups > 0;
}

bool NorthPole::Reindeer()
{
    return numBack == reindeer;
}



int NorthPole::Sleep()
{
    int ret = 0;
    
    MonitorBegin();
    printf("Santa ret to sleep!\n");
    santaState = SLEEP;
    //santaBusy->Signal();
    if (numBack != reindeer && numElfGroups != 1)
        wakeSanta->Wait();

    printf("YAWN!e: %d  r: %d\n", numElfGroups, numBack);
    if (numBack == reindeer) {
        santaState = REIN;
        santaBusyr->Signal();      
        //waitReindeer->Signal(); 
    } else {
        santaState = ELVES;
        santaBusy->Signal();
    }
    /**if (numBack != reindeer) {
        printf("Santa asleep!\n");
        wakeSanta->Wait();
    }**/
    //printf("santa is asleep!\n");
    /*santaOpen = 1;
    if (numBack != reindeer) {
        //elfP = 1;
        santaBusy->Signal();
    } else {
        santaOpen = 0;
        santaBusyr->Signal();
    }
    if (umElfGroups < 1 && numBack != reindeer) {
        //santaSleep = 1;
      //  printf("Santa asleep1\n");
        wakeSanta->Wait();
        //santaSleep = 0;
    } 

    if (numBack == reindeer && elfP == 0) {
        ret = 'r';
        //santaOpen = 1;
        //printf("reindeer woke me\n");  
        //santaBusyr->Signal();
    } else if (umElfGroups > 0) {
        santaOpen = 0;
        elvesWaiting = 0;
        umElfGroups = 0;
        otherElves->Signal();
        //printf("Elves woke me!\n");
        //umElfGroups = 0; 
        ret = 'e';
    } else {
        //printf("what?!!!\n");
    //    printf("Elves: %d  r: %d of %d\n", elvesWaiting, numBack, reindeer);
    }
    //printf("Santa is awake - whoever!\n");
    //printf("Elves: %d  r: %d of %d\n", elvesWaiting, numBack, reindeer);
*/
    MonitorEnd();    
    return ret;
}

void NorthPole::GatherReindeer()
{
    int i = 0;
    MonitorBegin();
    printf("wait rs %d\n", waitOthers);
    if (waitOthers != reindeer)
        allWaiting->Wait();
    waitOthers = 0;
    waitReindeer->Signal();
//    printf("Santa is gathering the reindeer\n");
    if (gathered != reindeer)
        allGathered->Wait();
   
    if (!done) 
        printf("Santa is preparing sleighs\n");
    gathering->Signal();
    //printf("Deer being harnessed\n");
    if (harnessed != reindeer)
        allHarnessed->Wait();
    harness->Signal();
    //printf("Santa is waiting for the reindeer to fly\n");
    if (numFlying != reindeer)
        allFlying->Wait();
    flying->Signal();
    // present deliveries
    printf("The team flies off (%d)!\n", ++toysDelivered);
    MonitorEnd();   
}

void NorthPole::ReleaseReindeer(int *n)
{
    int i = 0;
    MonitorBegin(); 
    if (onHoliday != reindeer)
        allOnHoliday->Wait();
    *n = toysDelivered;
    resetReinVars();
    //santaOpen = 1;
    vacation->Signal();
    santaState = SLEEP;
    santaBusy->Signal();
    MonitorEnd();
}

Santa::Santa(NorthPole *np, int td)
{
    liz = np;
    toyDeliveries = td;
    //liz->PrintMe("%sSanta thread starts\n", 0, NULL);
}

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

void Santa::ThreadFunc()
{
    int i, reason;
    Thread::ThreadFunc();
    //printf("buzz\n");
    liz->PrintMe("%sSanta thread starts\n", 0, NULL);
    while (i < toyDeliveries)
    //for (i = 0; i < toyDeliveries; i++)
    {
        //Delay();
        //Delay();
        reason = liz->Sleep();
        if (liz->WhoWokeMe() == 'r')
        {
            liz->GatherReindeer();
            Delay();
            liz->ReleaseReindeer(&i);
        }
        // TODO: does santa answer any elves waiting when he gets back before 
        // going to sleep?
        Delay();
        Delay();
        Delay();
        //printf("reason: %d %d\n", reason, 'e');
        //liz->PrintMe("blah reason: %c\n", -1 * reason, NULL);
        if (liz->ElfGroups())
        {
            //liz->PrintMe("blah blah blah\n", 0, NULL);
            liz->AdmitElves();
            
            liz->PrintMe("Santa is helping elves\n", 0, NULL);
            Delay();
            Delay();
            Delay();
            //liz->PrintMe("elves are with santa\n", 0, NULL);
            liz->ReleaseElves();
        }
    }
    liz->DeliveriesDone();
    exit(0);
}

Elf::Elf(int i, NorthPole *np)
{
    liz = np;
    id = i;
}

void Elf::ThreadFunc()
{
    Thread::ThreadFunc();
    liz->PrintMe("%10sElf %d starts\n", -1 * id, NULL); 
    while(true)
    {
        Delay();
        liz->AskQuestion(id);
           // Exit();
        Delay();
    }
}

Reindeer::Reindeer(int i, NorthPole *np)
{
    id = i;
    liz = np;
}

void Reindeer::ThreadFunc()
{
    Thread::ThreadFunc();
    liz->PrintMe("%5sReindeer %d starts\n", -1 * id, NULL);
    while(true)
    {
        Delay();
        if (liz->ReindeerBack(id))
            Exit();
        if (liz->WaitOthers(id))
            Exit();
        if (liz->WaitSleigh(id))
            Exit();
        if (liz->FlyOff(id))
            Exit();
        Delay();
    }
}
