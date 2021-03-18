/* ------------------------------------------------------------------------- */
/* NAME: Sarah Larkin                                   Username: selarkin   */
/* DUE DATE: 03 APRIL 2020                                                   */
/* PROGRAM ASSIGNMENT 4                                                      */
/* FILE NAME: thread-main.cpp                                                */
/* PROGRAM PURPOSE:                                                          */
/*    Runs the main program for the landlord-students  simulation. Reads in  */
/*    arguments, creates the room, landlord, and students, begins and joins  */
/*    on the threads.                                                        */
/*-------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "thread.h"

/* ------------------------------------------------------------------------- */
/* FUNCTION: main                                                            */
/*   Runs the whole program                                                  */
/* PARAMETER USAGE:                                                          */
/*   argc: the number of command-line arguments                              */
/*   argv: array of command-line arguments as strings                        */
/* FUNCTION CALLED:                                                          */
/*   Room:  creates a room with all the synchronization needed               */
/*   Landlord: creates the landlord                                          */
/*   Student: creates a student                                              */
/* ------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    int m = 5;
    int n = 5;
    int k = 10;
    int i = 0;

    /* check for proper number of command-line args */
    if (argc != 4)
    {
        printf("There need to be %d command-line args\n", 3);
        return 1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    k = atoi(argv[3]);

    /* if any of the arguments are zero, set the default values */
    if (m == 0)
        m = 5;
    if (n == 0)
        n = 5;
    if (k == 0)
        k = 10;

    /* Create the room and landlord and students*/
    Room *room = new Room(k);
    Landlord *landlord = new Landlord(m, n, k, room);
    Student **students = (Student **) malloc(sizeof(Student *) * k);
    for (i = 0; i < k; i++)
        students[i] = new Student(i + 1, room);
    

    /* Start the threads */
    landlord->Begin();

    for (i = 0; i < k; i++)
        students[i]->Begin();

    /* Wait for the threads */
    for (i = 0; i < k; i++)
        students[i]->Join();
    
    landlord->Join();

    /* Delete the students, landlord, and room */
    for (i = 0; i < k; i++)
        delete(students[i]);


    delete(landlord);
    delete(room);
    return 0;
}
