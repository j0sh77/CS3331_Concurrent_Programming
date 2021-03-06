// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 04/03/2015                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread-main.cpp           
// PROGRAM PURPOSE :                                           
//    This program takes three integers as input and simulates
//  a game of baby eagles and a mother eagle. Each baby eagle
//  is its own thread and eats food in the specified number of
//  pots, which is fetched by the mother.
//
//    This file handles the setup of the game, creating and
//  executing the necessary threads.       
// ----------------------------------------------------------- 



#include <string.h>
#include <stdio.h>
#include "thread.h"

Mutex* mtx;
Mutex* filling;
Semaphore* finished;
Semaphore* refill;
Semaphore* wake;
Semaphore* pots;

int numPots = 0;
int numEating = 0;

// ----------------------------------------------------------- 
// FUNCTION  main :                        
//     This function is ran when the user executes the file.
//  It creates multiple threads to simulate the Hungry Eagles
//  game.                         
// PARAMETER USAGE :                                           
//    argc - the number of arguments passed
//    argv - an array of character arrays that represents all
//  command line arguments.               
// FUNCTION CALLED :                                           
//    sprintf, write, exit, atoi, motherThread->Begin,
//  motherThread->Join, babyThread->Begin, babyThread->Join,       
// -----------------------------------------------------------


int main(int argc, char* argv[])
{
	char output[200];
	//check for valid arguments
	if(argc != 4)
	{
		sprintf(output, "Invalid arguments. Usage: ./prog4 m n t\n");
		write(1, output, strlen(output));
		exit(0);
	}
	
	int m = atoi(argv[1]); //feeding pots
	int n = atoi(argv[2]); //eagles
	int t = atoi(argv[3]); //feedings

	if(m == 0)
		m = 10;
	if(n == 0)
		n = 10;
	if(t == 0)
		t = 10;

	babyThread* babies[n];
	motherThread* mother;
	int i;

	//create mutex/semaphores needed
	mtx = new Mutex("mtx");
	filling = new Mutex("filling");
	finished = new Semaphore("finished", 0);
	refill = new Semaphore("refill", 0);
	wake = new Semaphore("wake", 1);
	pots = new Semaphore("pots", 0);
	
	sprintf(output, "MAIN: There are %d baby eagles, %d feeding pots, and %d feedings.\nMAIN: Game started!!!!!\n", n, m, t);
	write(1, output, strlen(output));


	mother = new motherThread(t, m);
	
	for(i = 0; i < n; i++)
		babies[i] = new babyThread(i, m, mother);

	//start all threads
	mother->Begin();
	for(i = 0; i < n; i++)
		babies[i]->Begin();
	

	mother->Join();

	//wait for all babies to complete
	for(i = 0; i < n; i++)
		babies[i]->Join();
	

}