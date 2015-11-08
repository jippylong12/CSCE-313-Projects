#include "semaphore.h"
#include <stdlib.h>
#include<string>
#include<iostream>
#include <vector>
using namespace std;
class BB{ //bounded buffer
	Semaphore*  full; //consumer's constraints
	Semaphore* empty;// producer's constraints
	Semaphore* mut; // mutual exclusion
	vector<string> data;
public:
	BB(int b=10){
		full=new Semaphore(0);
		empty=new Semaphore(b);
		mut=new Semaphore(1);
	}
	void isempty();
	void push(string item);
	string pop();
};