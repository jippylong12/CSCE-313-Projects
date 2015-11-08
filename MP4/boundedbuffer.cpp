#include "boundedbuffer.h"
void BB:: push(string item){
	empty->P();
	mut->P();
	data.push_back(item);
	mut->V();
	full->V();
}

string BB::pop(){
	full->P();
	mut->P();
	string temp= data.back();
	data.pop_back();
	mut->V();
	empty->V();
	return temp;
}