#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "linked_list.h"

using namespace std;

void Linked_list::Init (int M, int b)
{
	//initialize with malloc
	head = (Node*) malloc(M);
	b_length = b;
	m_length=M;
	//point the free pointer to the beginning of our allocated memory
	pFree = head; // point to where the next node will be. 
	cout<<"b is"<<b<<endl;
	cout<<"M is"<< M<<endl;
	cout<< "head value="<<head <<endl;
	cout<<"pFree="<<pFree<<endl;
 	int temp = pFree - head;
	cout<<"diff="<< temp <<endl<<endl;
}
void Linked_list::Destroy ()
{
	//free() or delete address
	free(head);
} 		 
int	Linked_list::Insert (int key,const char * value_ptr, int value_len)
{
		
	if(key<0)
	{
		cout<<"Please use a key 0 or greater \n"; return 0;
	} //no bad key values
	
	if(value_len > b_length )
	{
		cout<<"The size of value is bigger than b. \n"; return 0;
		
	}
	
	if(pFree+b_length > head+m_length)
	{
		cout<<"No more space to add node \n";
		return 0;
	}
	if(head==pFree) //this is the first node
	{
		cout<<"inserting a new node"<<endl;
		Node* new_node= head;
		pFree=head+b_length;
		new_node->key=key;
		new_node->vlen=value_len;
		new_node->next=pFree;
		memcpy((char*)head+16,value_ptr,b_length-16);
		new_node=0;
		delete new_node;
		cout<<"im the first node"<<endl<<endl;
	}
	else //not the first node
	{		
		Node* new_node= pFree; //new node goes to where free pointer is
		cout<<"inserting a new node"<<endl;
		new_node->key=key; //change the values
		new_node->vlen=value_len;
		pFree=pFree+b_length; //move the free pointer up
		new_node->next=pFree; //assign the next to the next add location
		memcpy((char*)new_node+16,value_ptr,b_length-16); //add the value 
		new_node= 0; //clean up
		delete new_node;
	}
	//places key, value length, and value using memcpy by adding 16 to the address of the free pointer. 
	//next pointer set to null
	//if there is an item previous then change the next pointer
	//the memory is full when the free pointer points to memory + M
}
int	Linked_list::Delete (int key)
{
	if(key<0) { cout<<"Please use a key 0 or greater \n"; return 0;}
	Node* nav; //traverse the list
	nav = this->head; //place at beginning
	while(key != nav->next->key) // we need to look ahead because we can't go back
	{
		nav = nav->next; // go to the next node
		if(nav->next == 0){	break;}
	}
	//once we break then we are either at the end of the list or we found the key
	if(nav->next == 0)
	{
		cout<<"Key "<<key<<" not found for deletion.\n \n";
		return 0;
	}
	else
	{
		//set all values to zero
		nav->next->key = -1; //change the key to something we won't be able to access
		nav->next->vlen = 0;
		if(nav->next->next == 0) //we are deleteing the last item
		{
			nav->next = pFree; //point to the next block
			return 1;
		}
		else // we are in the middle
		{
			Node* temp = nav->next->next; //create a new node to point to where we want to change
			nav->next->next = 0; //change the deleted node to point to null
			nav->next = temp; //reassign the pointers
			temp = 0; //temp points to nothing
			delete temp; //clean up 
		}
		return 1;
	}
	
}
char* 	Linked_list::Lookup (int key)
{
	if(key<0) { cout<<"Please use a key 0 or greater \n"; return 0;}
	Node* nav; //traverse the list
	nav = head; //place at beginning
	while(key != nav->key)
	{
		nav = nav->next; //traverse until we found key
		if(nav->next == 0) //end of list break
			break;
	}
	//again when we break we have either found the key or we are at the end of the list
	if(nav->next == 0 && nav->key != key) //if we are at the end of the list and the key does not match what we want
		return 0; //return failure
	else
	{
		char* temp = (char*)nav +8;
		return temp; //returns node
	}
}
void 	Linked_list::PrintList ()
{
 	Node* nav = this->head;
	cout<<endl<< "This is the head"<<endl;
	//cout <<"key:"<< head->key <<'\t'<<"value length:"<<head->vlen<<endl;
	while(nav->next!= 0)
	{
		cout<< "New Node"<<endl;
		cout <<"key:"<< nav->key <<'\t'<<"value length:"<<nav->vlen<<endl;
		//char* temp = (char*) nav +16; where the value would be
		nav = nav->next;
	} 
	
}