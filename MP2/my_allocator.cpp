/* 
    File: my_allocator.cpp

    Author: Marcus Salinas & Austin Tang
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/


//Problems to Address
//1) The formula does not go look to next tier
//for instance we call 3 my_malloc(100) it will say there is
//no more room in the list because it does not know to go to
//next tier to look for the block. 


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>
#include<stdlib.h>
#include "my_allocator.h"
#include "freeList.h"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */
memoryManager myMemory; //create abstract idea
/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/



//bug fixing
void printList(std::vector<Header*> list)
{
    Header* print; //for navigation
    int counter; //for counting blocks in index
    for(int i = 0; i<myMemory.sizeOfList; ++i) //for all indexes
    {
        //assign print
        print = myMemory.freeList[i];
        //set counter to 0 
        counter = 1;
        //print out contents 
        std::cout<<"Index: "<<i<<std::endl<<std::endl;
        while(print!= 0)
        {
            std::cout<<"Block number: "<<counter
                <<" is_Free: "<<print->is_free
                <<" size: "<<print -> size
                <<" Address of Itself: "<<print
                <<" Next Address: "<<print->next<<std::endl;
            counter++;
            //if we are at the end
            if(print->next == 0) {break;}
            //otherwise go to the next one
            print = print ->next;
            
        }
    }
}



/* Don't forget to implement "init_allocator" and "release_allocator"! */

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length) //return the memory manager
{
    //create the free list here

    myMemory.startOfList = malloc(_length); //create the memory block and assign start of memory
    std::cout<<"The memory starts at "<<myMemory.startOfList<<std::endl;
    //set the myMemory values 
    myMemory.basicBlockSize = _basic_block_size;
    myMemory.length = _length;
    myMemory.sizeOfList = floor(log2(_length/_basic_block_size)) + 1; //the size of the list will be logarithmic funciton + 1
    
    int size = myMemory.sizeOfList; //for simplicity
    
    //resize the free list vector to correct size
    myMemory.freeList.resize(size);
    //set the pointer to the correct index
    myMemory.freeList[size - 1] = (Header*)myMemory.startOfList;
    

    //sets the header node size of that node
    myMemory.freeList[size -1] -> size = (pow(2.0,size-1)*myMemory.basicBlockSize);
    //set the is_Free
    myMemory.freeList[size -1 ] ->is_free = true; // memory is free to us
    
    return 1;
}

int release_allocator()
{
    //free the inital address of the allocated memory
    free(myMemory.startOfList); //free all the memory 
    return 1;
}

extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  //return malloc((size_t)_length);
  //std::cout<<"Size of: "<<sizeof(myMemory.freeList[2]);
 //printList(myMemory.freeList);
  std::cout<<"Length Allocating: "<<_length<<std::endl;
  //checks to see if we can use that length
  if(_length + 8 > myMemory.length) 
  {
      std::cout<<"The size requested is larger than the size allocated. \n";
      return 0;
  }
  
  int indexCheck; // for seeing what index to check 
  double a = log2(((double)_length + 8)/(double)myMemory.basicBlockSize);
  std::cout<<"log2: "<<a<<std::endl;
  //if the length is less than basic block size we give it
  //basic block size
  if(_length + 8  <= myMemory.basicBlockSize)
  {
      indexCheck = 0; //the basic block size will be in 0
  }
  //the formula doesn't work for the values less than 1 because of log
  //after the first multiple it should work
  else
  {
    indexCheck = ceil(log2(((double)_length + 8)/(double)myMemory.basicBlockSize));
  }
  bool indexFound = false; //when we find the correct size
  //look for the correct index in the free list
  while(!indexFound)
  {
      //if there is no node in the free list we need to
      //go up until we find a node
        
      if(myMemory.freeList[indexCheck] == 0)
      {
          indexCheck+= 1;
      }
      //else we are in the right list 
      else
      {
            indexFound=true;
      }
  }
  
  //now we need to return the address of block with what
  //we have set up. We need to find a free block in the 
  //index with the smallest size possible. 
  
    bool foundBlock = false;
    //we need to navigate the list
    //create and set pointer to navigate
    Header* navigate;
    Header* navigate2;

    //go until we have found a block
 while(!foundBlock)
 {
  
    //printList(myMemory.freeList);
  
    //start at the begining of the list
    bool nextTier = false; //if we need to go to next tier
    navigate = myMemory.freeList[indexCheck];
    navigate2 = 0;
    //while we haven't found a free block
    std::cout<<"Index Check: "<<indexCheck<<std::endl;
    
    while(myMemory.freeList[indexCheck] == 0)
     {
      //if there is no node in the free list we need to
      //go up until we find a node
      indexCheck+=1;
      navigate = myMemory.freeList[indexCheck];
      
     }
    
    while(navigate -> is_free != true)
    {
        //check if there is a next node
        
        // std::cout<<"index "<<indexCheck<<" is_free "<<navigate->is_free<<std::endl;
        //if we get to the last tier and there are no more blocks
    std::cout<<"Here 2: \n";
        if (navigate ->next == 0 && indexCheck >= myMemory.sizeOfList)
        {
            std::cout<<"No more free blocks \n";
            return 0;
        }
        if(navigate ->next == 0) 
        {
            std::cout<<"Checking next tier \n"; 
            indexCheck++; //increase tier
            nextTier = true;
            break;
        } 
        //go to the next block of memory
        navigate2 = navigate;
    
        navigate = navigate->next;
    }
    
    if(nextTier) continue;
    //once we exit we are at the right node to use
    //now we need to make sure it's the correct size
    //we do that by checking the length request + length
    //of the header and compare it to the size of the block
    //at the index we are at
    //Ex: we want a length of 30 + 1 and the indexCheck should
    //place us at the next available free block so let's say
    //our basic size is 128 and our total length is 1024
    //and there is only one block of 1024. This puts the index
    //at 3 so if mulitiply 3+1 * 128 * 2 we get 1024 so we only
    //need the basic block size so will will need to split until
    //we have that size available
    
    int tempSize = myMemory.freeList[indexCheck] -> size; // for simplicity
    if(_length+16 > myMemory.basicBlockSize && indexCheck == 0)
    {
        std::cout<<"There is no availabe room for your request. \n";
        return 0;
    }
    //the size is less than basic block and we are at the right index
    if(_length + 8 < myMemory.basicBlockSize && indexCheck == 0)
    {
        navigate->is_free = false;
        char* arithmetic_address=reinterpret_cast<char*>(navigate) +16;// safe arithmetic avoiding void arithmetic bc it gives warnings
        void* address =static_cast<void*>(arithmetic_address); //16 becuase that's the size of header
        /*navigate should be at the right block*/
        std::cout<<"GIVING AWAY:size "<<navigate->size<<" is_free "<<navigate->is_free<<"Location: "<<navigate
            <<" index "<<indexCheck<<std::endl;
        return address;
        
    }   
    
    std::cout<<"Power Check: "<<pow(2.0,indexCheck -1)<<std::endl;
    
    //if it is bigger than the limit of the lower tier and smaller than the
    //limit of the current tier we are in then it is at the right spot
    if(_length + 8 > pow(2.0, indexCheck -1)* myMemory.basicBlockSize &&
        _length + 8 <= pow(2.0,indexCheck)*myMemory.basicBlockSize)
        {
            navigate->is_free=false;
            std::cout<<"GIVING AWAY:size "<<navigate->size<<" is_free "<<navigate->is_free<<"Location: "<<navigate
                    <<" index "<<indexCheck<<std::endl;
            char* arithmetic_address=reinterpret_cast<char*>(navigate) +16;// safe arithmetic avoiding void arithmetic bc it gives warnings
            void* address =static_cast<void*>(arithmetic_address); //16 becuase that's the size of header
            return address; //navigate should be in the right tier
        }
    //if we get to this point we need to split it up into two 
    //and send them down to the lower tier
    char* split; //for the next 
    //if the list is empty assign it 
    
    if(myMemory.freeList[indexCheck - 1] == 0)
    {
        
        myMemory.freeList[indexCheck - 1] = navigate; //send the block down
        
        //we need to keep all the other unused blocks in the correct tier
        //if there is mroe than one block in the list
        if(navigate2 != 0)
        {
             navigate2->next = navigate -> next;
        }
        //there is only one block in the list
        else
        {
            myMemory.freeList[indexCheck] = navigate ->next;
        }
        //set next to 0 for safety
        
        myMemory.freeList[indexCheck -1 ] -> next = 0;
        
        //this should cut it into two pieces
        split = reinterpret_cast<char*>(myMemory.freeList[indexCheck - 1]); 
        //need to add by char* or else it will add by HEADER block size
        split=split+tempSize/2;
        // std::cout<< "split size " << tempSize/2 << std::endl;
        // std::cout<<"address of the previous"<< myMemory.freeList[indexCheck-1]<<std::endl;
        // std::cout<<"address of the split"<<static_cast<void*>(split)<<std::endl;
        //set the next to the point of the split
        myMemory.freeList[indexCheck - 1] ->next =(Header*) split;
        //change size values
        myMemory.freeList[indexCheck - 1] -> size = tempSize/2;
        myMemory.freeList[indexCheck - 1] ->next -> size = tempSize/2;// changed from split to this because I changed split to a char*
        myMemory.freeList[indexCheck - 1] ->next ->is_free = true;
        myMemory.freeList[indexCheck - 1] -> next->next = 0;
        
    }
    //otherwise the list is not empty we will append it to the end
    else
    {
        Header* endOfList = myMemory.freeList[indexCheck - 1];
        while(endOfList -> next != 0)
        {
            endOfList = endOfList ->next;
        }
        endOfList -> next = navigate;
        if(navigate2 != 0)
        {
             navigate2->next = navigate -> next;
        }
        //there is only one block in the list
        else
        {
            myMemory.freeList[indexCheck] = navigate ->next;
        }
        //set next to 0 for safety
        navigate -> next = 0;
        split = reinterpret_cast<char*>(endOfList -> next); 
        split=split+tempSize/2;
         std::cout<< "split size " << tempSize/2 << std::endl;
         std::cout<<"address of the previous "<<navigate <<std::endl;
         std::cout<<"address of the split "<<static_cast<void*>(split)<<std::endl;
        //set the next to the point of the split
        endOfList ->next ->next =(Header*) split;
        std::cout<<"address of:  "<<endOfList->next->next <<std::endl;
        endOfList -> next -> size = tempSize/2;
        endOfList ->next -> next -> size = tempSize/2;// changed from split to this because I changed split to a char*
        endOfList ->next -> next -> is_free = true;
        endOfList ->next->next->next = 0;
    }
    indexCheck--; //decrement index check and go through the loop again.
 }
    
}
  
  

extern int my_free(Addr _a) {
  /* Same here! */
  
  //printList(myMemory.freeList);
  char* freed_mem=reinterpret_cast<char*>(_a)-16; //-16 to get the header of the address
  Header* new_mem=reinterpret_cast<Header*>(freed_mem); // casting to header to check for information of block of memory 
  std::cout<<"Freeing: "<<new_mem<<std::endl;
  std::cout<<"\n\n\n\n";
  /* couts for testing puposes*/
//   std::cout<<"size: "<< new_mem->size<<std::endl;
//   std::cout<<"is_free: "<< new_mem -> is_free<< std::endl;
//   std::cout<<"next: "<< new_mem-> next<< std::endl;
  int indexCheck; // for seeing what index to check 
  new_mem->is_free=true;
  //if the length is less than basic block size we give it
  //basic block size
  
  if(new_mem->size== myMemory.basicBlockSize)
  {
      indexCheck = 0; //the basic block size will be in 0
  }
  //the formula doesn't work for the values less than 1 because of log
  //after the first multiple it should work
  else
  {
    indexCheck = ceil(log2((new_mem->size)/myMemory.basicBlockSize)); 
  }
//   std::cout<<"memory of the first node in the free list"<<myMemory.freeList[indexCheck]<<std::endl;

//   std::cout<<"index"<<indexCheck <<std::endl;
  Header* lead = 0; //used to navigate free list
  Header* follow; //used to connect the linked list after a move
  Header* follow2; //used to connect free list
  
  //need that type for arthimitic
  //unsigned long long temp_number=(unsigned long long)new_mem-(unsigned long long)myMemory.startOfList; //for arithmitic XOR 
  for(int i=indexCheck;i<myMemory.freeList.size(); i++) // iterate through the freelist starting from the new node
  { 
      bool combine = false;// did we combine
      lead=myMemory.freeList[indexCheck]; //start from the first header in that specific freeList
      follow=0;
      follow2=0;
    //   std::cout<<"lead"<<lead<<" new_mem"<<new_mem<<std::endl;
      unsigned long long temp_number=(unsigned long long)new_mem-(unsigned long long)myMemory.startOfList; //for arithmitic XOR
      //change lead follow and follow2 after each while iteration
      //while not at the end of the list
      while(lead !=0)
      {
          //always change temp_number as we go to each node in the free list
          unsigned long long temp_number2 = (unsigned long long) lead - (unsigned long long) myMemory.startOfList; // for arthimitic 
          std::cout<<indexCheck<<std::endl;
          std::cout<<"Temp_number: "<<temp_number<<" Temp_number 2: "<<temp_number2<<std::endl;
          //printf("XOR of temps: %lld \n ",temp_number^temp_number2);
          //if they are equal then they are not a buddy also no need to check if the block isn't free
            //if they are buddies and lead is free
            if((temp_number^temp_number2)==pow(2.0,(indexCheck))*myMemory.basicBlockSize && lead ->is_free)
            {
                std::cout<<pow(2.0,(indexCheck))*myMemory.basicBlockSize<<","<<(temp_number^temp_number2)<<std::endl;
                Header* insert_new;
                insert_new=myMemory.freeList[indexCheck+1];// for checking next tier
                //lead is on the right and new_mem is on the left
                if(temp_number<temp_number2)
                {
                    std::cout<<"Here 1 \n";
                    if(insert_new==0) //case if there is no nodes in the freeList above
                    {
                        insert_new=new_mem; //inserting into the block above
                        //creating one block
                        std::cout<<"Insert: "<<insert_new<<" Insert next : "<<insert_new->next<<std::endl;
                        std::cout<<lead->next<<std::endl;
                        if(insert_new->next!=0)
                        insert_new-> next = insert_new ->next ->next;
                        insert_new->size=insert_new -> size*2; //double the size before we move to next tier
                        //if there is a node after this one
                        if(lead -> next != 0)
                        {
                            //if follow2 is equal to 0 then we are at the front of the list
                            if(follow2 == 0 && lead -> next!=new_mem)
                            {
                                //make lead next the start of the tier's list
                               // myMemory.freeList[indexCheck] -> next = lead->next;
                               myMemory.freeList[indexCheck]= lead->next;
                            }
                            else if(follow2 == 0 && lead -> next==new_mem)
                            {
                                myMemory.freeList[indexCheck]= lead->next->next;
                            }
                            //if follow2 is equal to insert_new then we are in the middle of the list
                            else
                            {
                                //merge the two lists
                                follow2->next = lead->next;
                            }
                        }
                        //else there are no nodes after the two we are moving
                        else
                        {
                            //if follow 2 hasn't been assigned then we are at the front two blocks
                            if(follow2 == 0)
                            {
                                //so since we are at the front two and nothing is after
                                //we just set this to 0 
                                //myMemory.freeList[indexCheck] -> next = 0;
                                std::cout<<"changing to nothing"<<std::endl;
                                myMemory.freeList[indexCheck] = 0;
                            }
                            //if follow 2 is assigned we are in the middle of the list somewhere
                            else
                            {
                                //so since we are in the middle and there are no nodes
                                //after the ones we move then we just set the node's next before
                                //the two that move to 0
                                follow2->next = 0;
                            }
                        }
                        insert_new ->next = 0;
                        lead->next=0;
                        combine = true; // so we continue to check the other tiers
                        myMemory.freeList[indexCheck+1]=insert_new;
                        std::cout<<"test"<<myMemory.freeList[indexCheck+1]->size<<std::endl;
                        std::cout<<"1CHANGING TO the index "<<indexCheck+1<<std::endl;
                        //printList(myMemory.freeList);
                        std::cout<<"\n\n\n\n";
                        break;
                    }
                    //there is one or more nodes in the list above
                    else 
                    {
                        Header* temp;
                        //we need to hold onto where that position is 
                        temp = myMemory.freeList[indexCheck + 1]; 
                       // we need to get to end of list
                        while(temp->next != 0)
                        {
                            temp = temp->next;
                        }
                        temp-> next = new_mem;
                        ///creating one block
                        if(temp->next->next!=0)
                          temp -> next -> next = temp -> next -> next -> next;
                        else
                          temp->next->next=0;
                        temp->next->size=temp-> size; //double the size before we move to next tier
                        //rearrage the pointer for the tier we just left
                        //if there are still nodes in the tier we moved from we need to not lose them
                        if(lead -> next != 0)
                        {
                            //if follow2 is equal to 0 then we are at the front of the list
                            if(follow2 == 0 && lead -> next!=new_mem)
                            {
                                //make lead next the start of the tier's list
                                //Memory.freeList[indexCheck] -> next = lead->next;
                                myMemory.freeList[indexCheck]=lead->next;
                            }
                            else if(follow2 == 0 && lead -> next==new_mem)
                            {
                                myMemory.freeList[indexCheck]= lead->next->next;
                            }
                            //if follow2 is equal to insert_new then we are in the middle of the list
                            else
                            {
                                //merge the two lists
                                follow2->next = lead->next;
                            }
                        }
                        //else there are no nodes after the two we are moving
                        else
                        {
                            //if follow 2 hasn't been assigned then we are at the front two blocks
                            if(follow2 == 0)
                            {
                                //so since we are at the front two and nothing is after
                                //we just set this to 0 
                                //myMemory.freeList[indexCheck] -> next = 0;
                                myMemory.freeList[indexCheck]= 0;
                            }
                            //if follow 2 is assigned we are in the middle of the list somewhere
                            else
                            {
                                //so since we are in the middle and there are no nodes
                                //after the ones we move then we just set the node's next before
                                //the two that move to 0
                                follow2->next = 0;
                            }
                        }
                        //rearrage the tier we are going to 
                        combine = true;
                        lead->next=0;
                        myMemory.freeList[indexCheck+1]=insert_new;
                        std::cout<<"test"<<myMemory.freeList[indexCheck+1]->size<<std::endl;
                        std::cout<<"2CHANGING TO the index 1"<<indexCheck+1<<std::endl;
                        //printList(myMemory.freeList);
                        std::cout<<"\n\n\n\n";
                        break; // so we continue to check the other tiers
                    }
                }
                //lead is on left and new mem is right
                else if(temp_number>temp_number2)
                {
                    if(insert_new==0) //case if there is no nodes in the freeList above
                    {
                        insert_new=lead; //inserting into the block above
                        //create one block
                        if(insert_new->next!=0)
                            insert_new-> next = insert_new ->next ->next;
                        else
                            insert_new-> next =0;
                        insert_new->size=insert_new -> size*2; //double the size before we move to next tier
                        
                        //if there is a node after this one
                        //if there are still nodes in the tier we moved from we need to not lose them
                        if(new_mem -> next != 0)
                        {
                            //if follow2 is equal to 0 then we are at the front of the list
                            if(follow == 0)
                            {
                                //make lead next the start of the tier's list
                                //myMemory.freeList[indexCheck] -> next = insert_new->next;
                                myMemory.freeList[indexCheck] = insert_new->next;
                            }
                            //if follow2 is equal to insert_new then we are in the middle of the list
                            else
                            {
                                //merge the two lists
                                follow->next = new_mem ->next;
                            }
                        }
                        //else there are no nodes after the two we are moving
                        else
                        {
                            //if follow 2 hasn't been assigned then we are at the front two blocks
                            if(follow == 0)
                            {
                                //so since we are at the front two and nothing is after
                                //we just set this to 0 
                                myMemory.freeList[indexCheck]  = 0;
                            }
                            //if follow 2 is assigned we are in the middle of the list somewhere
                            else
                            {
                                //so since we are in the middle and there are no nodes
                                //after the ones we move then we just set the node's next before
                                //the two that move to 0
                                follow->next = 0;
                            }
                        }
                        combine = true; // so we continue to check the other tiers
                        new_mem->next=0;
                        new_mem=lead;
                        insert_new->next = 0;
                        myMemory.freeList[indexCheck+1]=insert_new;
                        std::cout<<"test"<<myMemory.freeList[indexCheck+1]->size<<std::endl;
                         std::cout<<"\n\n\n\n";
                        std::cout<<"3CHANGING TO the index "<<indexCheck+1<<std::endl;
                        //printList(myMemory.freeList);
                        std::cout<<"\n\n\n\n";
                        break;
                    }
                    //there is one or more nodes in the list above
                    else 
                    {
                        //we need to hold onto where that position is 
                        Header* temp;
                        temp = myMemory.freeList[indexCheck + 1]; 
                        // we need to get to end of list
                        while(temp->next != 0)
                        {
                            temp = temp->next;
                        }
                        temp -> next =lead; //inserting into the block above at end of list
                        //create one block
                        temp-> next -> next = temp ->next ->next ->next;
                        temp->next->size=temp -> size; //double the size before we move to next tier
                        //rearrage the pointer for the tier we just left
                        //if there are still nodes in the tier we moved from we need to not lose them
                        if(new_mem -> next != 0)
                        {
                            //if follow is equal to 0 then we are at the front of the list
                            if(follow == 0)
                            {
                                //make lead next the start of the tier's list
                                myMemory.freeList[indexCheck] -> next = new_mem ->next;
                            }
                            //if follow is equal to insert_new then we are in the middle of the list
                            else
                            {
                                //merge the two lists
                                follow->next = new_mem -> next;
                            }
                        }
                        //else there are no nodes after the two we are moving
                        else
                        {
                            //if follow hasn't been assigned then we are at the front two blocks
                            if(follow == 0)
                            {
                                //so since we are at the front two and nothing is after
                                //we just set this to 0 
                                std::cout<<"h1 \n";
                                myMemory.freeList[indexCheck]  = 0;
                            }
                            //if follow 2 is assigned we are in the middle of the list somewhere
                            else
                            {
                                //so since we are in the middle and there are no nodes
                                //after the ones we move then we just set the node's next before
                                //the two that move to 0
                                follow->next = 0;
                            }
                        }
                        //rearrage the tier we are going to 
                        combine = true;
                        new_mem->next=0;
                        std::cout<<"lead"<<lead<<" new_mem"<<new_mem<<std::endl;
                        new_mem=lead;
                        std::cout<<"lead"<<lead<<" new_mem"<<new_mem<<std::endl;
                        std::cout<<"test"<<myMemory.freeList[indexCheck+1]->size<<std::endl;
                        std::cout<<"4CHANGING TO the index 1"<<indexCheck+1<<std::endl;
                        //printList(myMemory.freeList);
                        std::cout<<"\n\n\n\n";
                         std::cout<<lead->next<<std::endl;
                        break; // so we continue to check the other tiers
                    }
                   // new_mem=lead; // now the right address is in the front
                }
                else
                {
                    std::cerr<<"something bad happened!"<<std::endl;
                    return 1;
                }
            }
            
        //move the pointers in the list
        follow2=follow;
        follow=lead;
        lead=lead->next;  
      }
      
      if(!combine)
      {
          std::cout<<"im here"<<std::endl;
          break;
      }
      indexCheck++;
  }
  std::cout<<"Result after free: \n";
  printList(myMemory.freeList);
  return 1;
}
