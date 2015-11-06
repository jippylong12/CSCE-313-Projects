#include "my_allocator.h"
#include "ackerman.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include<stdlib.h>

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)
  init_allocator(128, 134217728);
  
  // void * mem = my_malloc(200);
  // void * mem2 = my_malloc(200);
  // void * mem3 = my_malloc(200);
  // void * mem4 = my_malloc(100);
  // void * mem5 = my_malloc(300);
  ackerman_main();

  // my_free(mem);
  // my_free(mem3);
  // my_free(mem2);
  // my_free(mem4);
  // my_free(mem5);
  
// void* mem5 = my_malloc(200);
// void* mem6 = my_malloc(100);
 
 
  release_allocator();
}
