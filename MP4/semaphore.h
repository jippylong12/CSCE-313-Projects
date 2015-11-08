/* 
    File: semaphore.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _semaphore_H_                   // include file only once
#define _semaphore_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/


//class Bounded buffer
//  Sema full(0)
//  Sema empty(b) //b is global variable. 
//  Sema mut(1)
//  vector<string> data;
//  void push()
//  string pop()


//void BB:push(string item)
//  empty.P() //will wait for open slot 
// mut.P()
//  data.push_back(item)
// mut.V()
//  full.V()




class Semaphore {
private:
  /* -- INTERNAL DATA STRUCTURES
     You may need to change them to fit your implementation. */

  int counter;
  pthread_mutex_t m;
  pthread_cond_t  c; //condition

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */

  Semaphore(int _val);
  ~Semaphore();

 
  /* -- SEMAPHORE OPERATIONS */

  int P(); //lock checks if counter is above zero or not
          //counter--
          //if(counter<0) wait(until counter becomes positive)
  int V(); //unlock
          //count++
          //signal() to send out that a resource is available now. 
};


#endif


