// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 03/20/2015                                       
// PROGRAM ASSIGNMENT #3                                        
// FILE NAME : thread-main.cpp           
// PROGRAM PURPOSE :                                           
//    This program takes an integer and an array as command
// line arguments. The program will find the index and value
// of the first non-negative integer in the array.
//
//    This is the main file that executes threads to handle
// the problem.       
// ----------------------------------------------------------- 



#include <string.h>
#include <stdio.h>
#include "thread.h"

 

// ----------------------------------------------------------- 
// FUNCTION  main :                        
//     This function is ran when the user executes the file.
//  it creates multiple threads to find the index and value
//  of the first non-negative integer in the passed array.                          
// PARAMETER USAGE :                                           
//    argc - the number of arguments passed
//    argv - an array of character arrays that represents all
//  command line arguments.               
// FUNCTION CALLED :                                           
//    atoi, initThread, initThread->Begin, initThread->Join,
//  elimThread, elimThread->Begin, elimThread->Join,
//  concThread, concThread->Begin, concThread->Join         
// -----------------------------------------------------------

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	int a[n];
	int w[n];
	int i, j, t = 0;
	int p = -1; //the result

	char output[200];

	//thread setup
	initThread* iThreads[n];
	elimThread* eThreads[n * (n - 1) / 2];
	concThread* cThreads[n];

	
	sprintf(output, "Number of input values = %d\n", n);
	write(1, output, strlen(output));

	//store passed array into a
	for(i = 0; i < n; i++)
	{
		a[i] = atoi(argv[i + 2]);
	}

	//print array
	sprintf(output, "Input values\t     a =");
	for(i = 0; i < n; i++)
		sprintf(output, "%s %d", output, a[i]);
	sprintf(output, "%s\n", output);
	write(1, output, strlen(output));

	//create pointers to array/result
	int* ap = a;
	int* wp = w;
	int* pp = &p;

	//initialize w
	for(i = 0; i < n; i++)
	{
		iThreads[i] = new initThread(i, w);
		iThreads[i]->Begin();
	}

	//wait for all iThreads to complete
	for(i = 0; i < n; i++)
		iThreads[i]->Join();
	
	//print w
	sprintf(output, "After initialization w =");
	for(i = 0; i < n; i++)
		sprintf(output, "%s %d", output, w[i]);
	sprintf(output, "%s\n", output);	
	write(1, output, strlen(output));



	//eliminate values. Compare all i and j pairs, where i < j
	for(i = 0; i < n; i++)
	{
		for(j = i + 1; j < n; j++)
		{
			eThreads[t] = new elimThread(ap, wp, i, j);
			eThreads[t]->Begin();
			t++;
		}
	}
	
	//wait for all eThreads to complete
	for(i = 0; i < t; i++)
		eThreads[i]->Join();

	//print w
	sprintf(output, "After elimination    w =");
	for(i = 0; i < n; i++)
		sprintf(output, "%s %d", output, w[i]);
	sprintf(output, "%s\n", output);	
	write(1, output, strlen(output));

	//find result
	for(i = 0; i < n; i++)
	{
		cThreads[i] = new concThread(i, w, pp);
		cThreads[i]->Begin();
	}

	//wait for all cThreads to complete
	for(i = 0; i < n; i++)
		cThreads[i]->Join();

	//print result
	sprintf(output, "Location:\t       = %d\n", p);
	write(1, output, strlen(output));
	sprintf(output, "Value\t\t       = %d\n", a[p]);
	write(1, output, strlen(output));




}
