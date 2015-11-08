#include "semaphore.h"

Semaphore::Semaphore(int _val){
    value=_val;
}
Semaphore::Semaphore(const Semaphore& s){
	
}
int Semaphore::P(){
    pthread_mutex_lock(&m);
    value--;
    if(value<0)
        pthread_cond_wait(&c,&m);
    pthread_mutex_unlock(&m);
}

int Semaphore:: V(){
    pthread_mutex_lock(&m);
    value++;
	if(value<0)
		pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);
}