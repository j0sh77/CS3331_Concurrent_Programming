// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 03/20/2015                                       
// PROGRAM ASSIGNMENT #3                                        
// FILE NAME : thread.cpp           
// PROGRAM PURPOSE :                                           
//    This program takes an integer and an array as command
// line arguments. The program will find the index and value
// of the first non-negative integer in the array.
//
//    This is the thread file that contains the code for each
// of the three types of threads: initialization, elimination,
// and conclusion.        
// ----------------------------------------------------------- 


#include <string.h>
#include <stdio.h>
#include "thread.h"


// ----------------------------------------------------------- 
// FUNCTION  initThread::initThread :                      
//     This function is executed by any initThread thread.
//  It initializes the values needed for this thread's job.                   
// PARAMETER USAGE :                                           
//     i - the integer index of w to initialize  
//     w - a pointer to the array w from the main process             
// FUNCTION CALLED :                                           
//        
// -----------------------------------------------------------

initThread::initThread(int I, int* W)//:i(I), w(W)
{
	this->i = I;
	this->w = W;
}

// ----------------------------------------------------------- 
// FUNCTION  initThread::ThreadFunc :                      
//     This function is executed by any initThread thread.
//  It initializes w[i] to the integer 1.                            
// PARAMETER USAGE :                                       
//           
// FUNCTION CALLED :                                           
//     Exit
// -----------------------------------------------------------

void initThread::ThreadFunc()
{
	Thread::ThreadFunc();
	int* w = this->w;
	int i = this->i;

	(*(w + i)) = 1; //w[i] = 1
	Exit();
};



// ----------------------------------------------------------- 
// FUNCTION  elimThread::elimThread :                         
//     This function is executed by any elimThread thread.
//  It initializes the values needed for this thread's job.                       
// PARAMETER USAGE :                                           
//     a - a pointer to the array a from the main process
//     w - a pointer to the array w from the main process
//     i - an index in a and w to compare against j
//     j - an index in a and w to compare against i             
// FUNCTION CALLED :                                           
//     
// -----------------------------------------------------------

elimThread::elimThread(int* A, int* W, int I, int J)//: a(A), w(W), i(I), j(J)
{
	this->a = A;
	this->w = W;
	this->i = I;
	this->j = J;
}

// ----------------------------------------------------------- 
// FUNCTION  elimThread::ThreadFunc :                         
//     This function is executed by any elimThread thread.
//  elimThread will determine which element(s) in w between 
//  i and j should be set to 0 (if any). It essentially
//  "eliminates" a potential value for the first non-negative
//  integer in a.                         
// PARAMETER USAGE :                                           
           
// FUNCTION CALLED :                                           
//     sprintf, write, Exit
// -----------------------------------------------------------

void elimThread::ThreadFunc()
{
	Thread::ThreadFunc();

	int* a = this->a;
	int* w = this->w;
	int i = this->i;
	int j = this->j;


	char output[200];
	sprintf(output, "Thread T(%d, %d) compares a[%d] = %d and a[%d] = %d, and writes", i, j, i, *(a + i), j, *(a + j));

	//i and j are both positive, w[j] = 0
	if (*(a + i) > 0 && *(a + j) > 0)
	{
		*(w + j) = 0;
		sprintf(output, "%s 0 into w[%d]\n", output, j);
	}
	//i and j are both negative, w[i] = 0 and w[j] = 0
	else if (*(a + i) < 0 && *(a + j) < 0)
	{
		*(w + i) = 0;
		*(w + j) = 0;
		sprintf(output, "%s 0 into w[%d] and w[%d]\n", output, i, j);
	}
	//i is positive and j is negative, w[j] = 0
	else if (*(a + i) > 0 && *(a + j) < 0)
	{
		*(w + j) = 0;
		sprintf(output, "%s 0 into w[%d]\n", output, j);
	}
	//i is negative and j is positive, w[i] = 0
	else if (*(a + i) < 0 && *(a + j) > 0)
	{
		*(w + i) = 0;
		sprintf(output, "%s 0 into w[%d]\n", output, i);
	}

	write(1, output, strlen(output));
	Exit();
};



// ----------------------------------------------------------- 
// FUNCTION  concThread::concThread :                          
//     This function is executed by any concThread thread.
//  It initializes the values needed for this thread's job.
// PARAMETER USAGE :                                             
//     i - an index in w to check for elimination
//     w - a pointer to the array w from the main process
//     p - a pointer to the integer p from the main process,
// 			which will contain the result of the program.               
// FUNCTION CALLED :                                           
//            
// -----------------------------------------------------------

concThread::concThread(int I, int* W, int* P)//:i(I), w(W), p(P)
{
	this->i = I;
	this->w = W;
	this->p = P;
}

// ----------------------------------------------------------- 
// FUNCTION  concThread::ThreadFunc :                          
//     This function is executed by any concThread thread.
//  concThread will determine if the value w[i] is not
//  eliminated. If it is not, the value of p is set to i.
// PARAMETER USAGE :
//              
// FUNCTION CALLED :                                           
//     Exit
// -----------------------------------------------------------

void concThread::ThreadFunc()
{
	Thread::ThreadFunc();
	int i = this->i;
	int* w = this->w;
	int* p = this->p;

	//set p = i if w[i] != 0
	if(*(w + i) != 0)
	{
		*p = i;
	}
	Exit();
};

