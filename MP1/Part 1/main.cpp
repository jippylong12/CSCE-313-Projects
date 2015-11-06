#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "linked_list.h"
#include <iostream>

using namespace std;

int main(int argc, char ** argv) 
{
	int c,b, M;
	b=128;
	M=512000;
	while ((c = getopt (argc,argv,"b:s:")) != -1)
	{
		switch(c)
		{
			case 'b':
				b = atoi(optarg);
				if(b==0){
					b=128;
				}
				break;
			case 's':
				M = atoi(optarg);
				if(M==0){
					M=51200;
				}
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
	list.Init (M,b); // initialize
	// test operations
 	int testnums [] = {100, 5, 200, 7, 39, 25, 400, 50, 200, 300};
	int i = 0;
	// some sample insertions
	for (i=0; i< 10; i ++)
	{
		list.Insert (testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	list.PrintList();
	cout<<endl<<endl;
	list.Delete(200);
	list.Delete(39);
	list.Delete(50);
	list.Delete(300);
	list.PrintList();
	list.Insert (150, buf, 200); // this Insert should fail
	list.PrintList ();
	list.Delete (7);
	list.Insert (13, msg, strlen(msg)+1);		// insertion of strings, copies the null byte at the end
	list.Delete (55);
	list.Insert (15, "test msg", 8);
	list.Delete (3);
	list.PrintList ();
	
	// a sample lookup operations that should return null, because it is looking up a non-existent number
	char* kv = list.Lookup (3);
	if (kv)
	 	printf ("Key = %d, Value Len = %d\n", *(int *) kv, *(int *) (kv+4));

	// this look up  should succeed and print the string "a sample message"
	kv = list.Lookup (13);
	if (kv)
		printf ("Key = %d, Value Len = %d, Value = %s\n", *(int *) kv, *(int *) (kv+4), kv + 8);
	// end test operations	
	list.Destroy (); 
	
}
