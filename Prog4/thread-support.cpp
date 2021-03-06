// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 04/03/2015                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread-support.cpp           
// PROGRAM PURPOSE :                                           
//    This program takes three integers as input and simulates
//  a game of baby eagles and a mother eagle. Each baby eagle
//  is its own thread and eats food in the specified number of
//  pots, which is fetched by the mother.
//
//    This file handles the detailed thread functions,
//  satisfying mutual exclusion principles and allowing
//  certain eagles to execute functions and the correct time.      
// ----------------------------------------------------------- 

#include <string.h>
#include <stdio.h>
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  babyThread::ready_to_eat :                      
//     This function is executed by a babyThread thread.
//  ready_to_eat() blocks the caller if feeding pots are
//  empty. One and only one baby eagle who finds out all
//  feeding pots are empty wakes the mother eagle.                         
// PARAMETER USAGE :                                       
//           
// FUNCTION CALLED :                                           
//     sprintf, write, Mutex->Lock, Mutex->Unlock, 
//  Semaphore->Signal(), Semaphore->Wait()
// -----------------------------------------------------------

void babyThread::ready_to_eat()
{
	char output[200];
	char spaces[25];
	int i;
	int n = this->i;

	//generate spaces
	sprintf(spaces, "");
	for(i = 0; i < n; i++)
		sprintf(spaces, "%s ", spaces);

	filling->Lock();
		mtx->Lock();

			//empty pots, signal mother
			if(numPots == 0)
			{
				if(this->mother->retiring)
				{
					mtx->Unlock();
					filling->Unlock();
					Exit();
				}

				else
				{
					this->mother->wokenBy = n;
					mtx->Unlock();
					//sprintf(output, "\t\tnumPots == 0    - Baby eagle %d: numEating = %d, numPots = %d.\n", n, numEating, numPots);
					//write(1, output, strlen(output));

					sprintf(output, "%sBaby eagle %d sees all feeding pots are empty and wakes up the mother.\n", spaces, n);
					write(1, output, strlen(output));		
					refill->Signal();
					pots->Wait();
					mtx->Lock();
				}
			}
	


			//one less available pot, one more eagle eating
			numPots--;
			numEating++;

			if(numPots < 0){
				mtx->Unlock();
				filling->Unlock();
				Exit();
			}

			sprintf(output, "%sBaby eagle %d is ready to eat.\n", spaces, n);
			write(1, output, strlen(output));

			//sprintf(output, "\t\tready_to_eat()  - Baby eagle %d: numEating = %d, numPots = %d.\n", n, numEating, numPots);
			//write(1, output, strlen(output));

			this->pot = this->m - numPots;

		mtx->Unlock();
	filling->Unlock();

};

// ----------------------------------------------------------- 
// FUNCTION  babyThread::finish_eating :                      
//     This function is executed by a babyThread thread.
//  finish_eating is called when a baby eagle finishes eating.
//                       
// PARAMETER USAGE :                                       
//           
// FUNCTION CALLED :                                           
//     sprintf, write, Mutex->Lock, Mutex->Unlock,
//  Semaphore->Wait()
// -----------------------------------------------------------

void babyThread::finish_eating()
{
	char output[200];
	char spaces[25];
	int i;
	int n = this->i;

	sprintf(spaces, "");
	for(i = 0; i < n; i++)
		sprintf(spaces, "%s ", spaces);

	mtx->Lock();
		//one less eagle eating now
		numEating--;

		//sprintf(output, "\t\tfinish_eating() - Baby eagle %d: numEating = %d, numPots = %d.\n", n, numEating, numPots);
		//write(1, output, strlen(output));
		
		sprintf(output, "%sBaby eagle %d finishes eating.\n", spaces, n);
		write(1, output, strlen(output));

		if(numEating == 0 && numPots == 0){
			finished->Signal();
			wake->Signal();
		}

	mtx->Unlock();

};

// ----------------------------------------------------------- 
// FUNCTION  motherThread::finish_eating :                      
//     This function is executed by the motherThread thread.
//  goto_sleep is called when the mother eagle wants to take
//  a nap.
//                       
// PARAMETER USAGE :                                       
//           
// FUNCTION CALLED :                                           
//     sprintf, write, Semaphore->Wait()
// -----------------------------------------------------------

void motherThread::goto_sleep()
{

	wake->Wait();
	refill->Wait();
	
};

// ----------------------------------------------------------- 
// FUNCTION  motherThread::food_ready :                      
//     This function is executed by the motherThread thread.
//  food_ready is called when the mother eagle has finished
//  adding food in all m feeding pots.
//                       
// PARAMETER USAGE :                                       
//           
// FUNCTION CALLED :                                           
//     sprintf, write
// -----------------------------------------------------------


void motherThread::food_ready()
{
	char output[200];
	sprintf(output, "Mother eagles says \"Feeding (%d)\".\n", i);
	write(1, output, strlen(output));

	
};




/*
		     NOT UPDATED			
 -------→|
 | 		 ↓	
 | -----→■ mtx
 | |	 |
 | |	 ↓
 | |  no pots? -------
 | |	 |			 ↓
 | |	 |			 o refill
 | |	 |			 ↓
 | |	 |←----------■
 | |     ↓
 | |  numPots--
 | | numEating++
 | |	 |
 | |     ↓
 | ------o mtx
 |		 |
 |       |
 |       ↓
 |	   eating
 |		 |
 |       |
 |       ↓
 |		 ■ mtx
 |       |
 |       ↓
 |   numEating--
 |	   last? ---------
 |  	 |			 ↓
 |  	 |			 o wake
 |  	 |			 ↓
 |       o mtx ←------
 |		 |
 |       |
 |       ↓
 |--------

*/
