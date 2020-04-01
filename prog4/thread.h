#include "ThreadClass.h"

class Landlord: public Thread
{

    public:
        Landlord(Semaphore *sq, Semaphore *d, Semaphore *e, Semaphore *sc, 
                Semaphore *eg, Semaphore *ex, Semaphore *b, 
                bool lr, bool lp, int ns, int sw, Room *r);
    private:
        void ThreadFunc();
        void CheckRoom();
        void printout(char *format, int n);

        Semaphore *studentQueue;
        Semaphore *door;
        Semaphore *entry;
        Semaphore *studentCount;
        Semaphore *egress;
        Semaphore *exited;
        Semaphore *bahamas;
        Semaphore *sout;

        bool landlordRetired;
        bool landlordPresent;
        int numStudents;
        int StudentsWaiting;             
        int m = 0; 

        char *p0 = "The landlord starts%s\n";
        char *p1 = "The landlord checks the room the %d-th time\n";
        char *p2 = "The landlord finds the room has no students " +
                   "and leaves%s\n";
        char *p3 = "The landlord finds there are %d students in the room "
                   + "(condition being good) and leaves\n";
        char *p4 = "The landlord finds there are %d students in the room "
                   + "and breaks up the party\n";
        char *p5 = "The landlord finishes checking and forces everyone "
                   + "to leave%s\n";
        char *p6 = "The landlord leaves the room and the room is empty%s\n";
        char *p7 = "After checking the room %d times, the landlord retires " 
                   + "and is on vacation!\n";
};

class Student: public Thread
{
    public:
        Student(Semaphore *sq, Semaphore *d, Semaphore *e, Semaphore *sc, 
                Semaphore *eg, Semaphore *ex, Semaphore *b, 
                bool lr, bool lp, int ns, int sw, Room *r);
    private:
        void ThreadFunc();
        Semaphore *studentQueue;
        Semaphore *door;
        Semaphore *entry;
        Semaphore *studentCount;
        Semaphore *egress;
        Semaphore *exited;
        Semaphore *bahamas;
        Semaphore *sout;

        bool landlordRetired;
        bool landlordPresent;
        int numStudents;
        int StudentsWaiting;     

        int studentNumber;
        char msg[16];
        char smsg = "%5sStudent %2d";
        char *p0 = "%s starts\n";
        char *p1 = "%s waits to enter the room\n";
        char *p2 = "%s enters the room (%2d students in the room)\n";
        char *p3 = "%s waits to leave the room\n";
        char *p4 = "%s leaves the room (%2d students in the room)\n";
        char *p5 = "%s terminates\n";
        
};

class Util
{
    public:
        static void printout(char *format, int n);
};

class Room
{
    public:
        bool landlordRetired = false;
        Semaphore *bahamas;
        Semaphore *door;// LN
        Semaphore *studentQueue;//LP
        Semaphore *entry;
        Semaphore *studentCount;
        int numStudents = 0;
        int studentsWaiting = 0;
        Semaphore *exit;
        Semaphore *studentsExited;
        Semaphore *sout;
        bool landlordPresent= false;
        Room(int bah, int d, int sq, int ent, int sc, int egress, int stuexit);
      private:
        int n;
};
