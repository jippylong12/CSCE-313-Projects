#include "boundedbuffer.h"
void BB:: push(string item){
	empty->P(); //decrement the empty buffer because we have one less 
	mut->P(); //
	data.push_back(item); //push the data we get back from the thread
	mut->V();
	full->V();//increment the full buffer
}

string BB::pop(){
	full->P(); //decrement the full buffer
	mut->P();
	string temp= data.back(); //get the data back
	data.pop_back(); //remove it because don't need it anymore
	mut->V(); 
	empty->V(); //increment the amount of free threads available. 
	return temp;
}