#include "semaphore.h"
#include <stdlib.h>
#include<string>
#include<iostream>
#include <vector>
#include <unistd.h>
using namespace std;
class BB{ //bounded buffer
	Semaphore*  full; //consumer's constraints
	Semaphore* empty;// producer's constraints
	Semaphore* mut; // mutual exclusion
	
public:
	BB(int b=100){
		full=new Semaphore(0);
		empty=new Semaphore(b);
		mut=new Semaphore(1);
	}
	vector<string> data;
	void isempty();
	void push(string item);
	int size();
	string pop();
};