#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "linked_list.h"

using namespace std;

int main(int argc, char ** argv) 
{
	int c;
	int b = 128;
	int M = b * 12;  // so we have space for 4 items in each of the lists
	int t = 4;		// 4 tiers
	while ((c = getopt (argc,argv,"b:s:t:")) != -1)
	{
		switch(c)
		{
			case 'b':
				b = atoi(optarg); //default
				if(b==0){
					b=128;
				}
				break;
			case 's':
				M = atoi(optarg); //default
				if(M==0){
					M=51200;
				}
				break;
			case 't':
				t = atoi(optarg);
				if(t == 0) t = 16; //basic values
				break;
			case '?':
				 if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
				
		}
	}
	char buf [1024];
	memset (buf, 1, 1024);		// set each byte to 1
	
	const char * msg = "a sample message";
	Linked_list list;
	list.Init (M, b, t); // initialize
	// test operations
	int testnums [] = {0, 1<<29 , (1<<29) + 5 , 50, (1<<30) + 5, (1<<30) - 500,400};
	int i = 0;
	// some sample insertions
	for (i=0; i< 10; i ++)
	{
		cout<<testnums[i]<<endl;   // insert 50 bytes from the buffer as value for each of the insertions
	}
	for (i=0; i< 10; i ++)
	{
		list.Insert (testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	list.PrintList ();
	list.Delete(1<<29);
		char* kv = list.Lookup (0);
	if (kv)
		printf ("Key = %d, Value Len = %d, Value = %s\n", *(int *) kv, *(int *) (kv+4), kv + 8);
	// end test operations	
	list.Destroy ();
	
}
