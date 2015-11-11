#include "semaphore.h"

Semaphore::Semaphore(){
    value=0;
	pthread_mutex_init(&m,NULL);
	pthread_cond_init(&c,NULL);
}
Semaphore::Semaphore(int _val){
    value=_val;
	pthread_mutex_init(&m,NULL);
	pthread_cond_init(&c,NULL);
}
Semaphore::~Semaphore(){
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
}
Semaphore::Semaphore(const Semaphore& s){
	
}
int Semaphore::P(){
    pthread_mutex_lock(&m);
    while(value<=0)
        pthread_cond_wait(&c,&m);
	value--;
    pthread_mutex_unlock(&m);
	return 0;
	
}

int Semaphore:: V(){
    pthread_mutex_lock(&m);
	value++;
	if(value>=1)
	{
		pthread_cond_signal(&c);
	}
	pthread_mutex_unlock(&m);
	return 0;
}