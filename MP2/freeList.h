#include <vector>
#include "my_allocator.h"
struct Header //for the header nodes
{
   bool is_free; //check if we can give it
   int size;  //we already know the size depending on index
   Header* next;
   
   //default constructor
   Header(bool iF = true, int s = 128, Header* n = 0)
        : is_free(iF), size(s), next(n) {}
};

struct memoryManager  //for the freelist function
{
    int basicBlockSize; //to keep track of basic block size
    int length; //total size of the alloc
    int sizeOfList; //size of free list
    Addr startOfList; //to easily delete
    std::vector<Header*> freeList; //the free list
    
    //default constructor 
    memoryManager(int bbs = 128, int l = 128, Addr SoL = 0, int size = 1)
        : basicBlockSize(bbs), length(l), startOfList(SoL), sizeOfList(size) {}
};
