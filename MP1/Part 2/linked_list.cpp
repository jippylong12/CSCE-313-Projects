#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //floor function
#include <iostream>
#include <vector>
#include "linked_list.h"

using namespace std;

void Linked_list::Init (int M, int b, int t)
{
	//initialize with malloc
	head = (Node*) malloc(M);
	tValue = t;
	b_length = b;
	m_length=M;
	for(int a = 0; a<t; ++a)
	{
		tiers.push_back(0);
	}
	checkTier = numberSpace/tValue;
	counterLimit =   m_length/b_length;
	//point the free pointer to the beginning of our allocated memory
	pFree = head; // point to where the next node will be. 
 	int temp = pFree - head;
}
void Linked_list::Destroy ()
{
	//free() or delete address
	free(head);
} 		 
int	Linked_list::Insert (int key,const char * value_ptr, int value_len)
{
	cout<<"Counter Limit"<<counterLimit<<endl;
	//error checking
	if(pFree+b_length > head+m_length)
	{
		cout<<"No more memory to add node \n"<<endl;
		return 0;
	}
	
	if(key<0)
	{
		cout<<"Please use a key 0 or greater \n"; 
		return 0;
	} //no bad key values
	
	if(value_len > b_length-16 )
	{
		cout<<"The size of value exceeds the block size. \n"; 
		return 0;
		
	}
	
	
	//we need a counter to limit inserts
	counter.resize(tValue);
	
	//take the key and divide it by checkTier and then floor it to get the array index
	int tierRank = key/checkTier;
	tierRank = floor(tierRank);
	
	if(head==pFree) //this is the first node
	{
		cout<<"inserting a new node"<<endl;
		Node* new_node= head; //set new node to head
		pFree=head+b_length; //adjust free pointer
		new_node->key=key; //assign values
		new_node->vlen=value_len;
		new_node->next=0; //point to where the next node shoudl go in our allocated memory
		head = new_node;
		memcpy((char*)head+16,value_ptr,b_length-16); //add the values
		tiers[tierRank] = new_node; //put the new node in the correct tier
		tiers[tierRank]->next = 0;
		counter[tierRank] = counter[tierRank] + 1; //increase the counter of nodes in this tier
		new_node=0; //deleteing the new node. 
		delete new_node;
		
		//testing
		cout<<"im the first node"<<endl<<endl;
		return 1;
	}
	else //not the first node
	{		
		cout<<"inserting a new node"<<endl<<endl;
		Node* new_node= pFree; //new node goes to where free pointer is
		new_node->key=key; //change the values
		new_node->vlen=value_len; 
		pFree=pFree+b_length; //move the free pointer up
		new_node->next=0; //assign the next to the next add location
		memcpy((char*)new_node+16,value_ptr,b_length-16); //add the value 
		if(tiers[tierRank] == 0) //this is the first node in the tier because it is pointing to NULL
		{
			
			tiers[tierRank] = new_node; //make this node the first node in the tier
			counter[tierRank] = counter[tierRank] + 1; //increase the counter of nodes in this tier
			return 1;
		}
		else //not the first node in a tier because it is not pointing to NULL
		{
			//if the counter for this rank is greater than what our limit is than we can't insert
			
			if(counter[tierRank] > counterLimit)
			{
				cout<<"There is no more room in that tier. \n";
				return 0;
			}
			//we want to make the new node insert at the beginning of the tier since order doesn't matter
			
			Node* temp= tiers[tierRank];
			while(temp->next!= 0)
			{
				temp = temp->next;
			}
			temp->next= new_node;
			
			counter[tierRank] = counter[tierRank] + 1; //increase the counter of nodes in this tier
			//PrintList();
			return 1;
		}
		new_node= 0; //clean up
		delete new_node;
		
		cout<<"im not the first node"<<endl<<endl;
	}
	//places key, value length, and value using memcpy by adding 16 to the address of the free pointer. 
	//next pointer set to null
	//if there is an item previous then change the next pointer
	//the memory is full when the free pointer points to memory + M
}
int	Linked_list::Delete (int key)
{
	if(key<0) { cout<<"Please use a key 0 or greater \n"; return 0;}
	/*Node* nav; //traverse the list
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
	*/
	for(int i=0; i<tValue;i++){ // traverse through each tier
		Node* temp =tiers[i];
    	if(temp!=0){
	 		while(temp->next!=0&& !is_same_tier(temp,tiers,i)){
	 				if(temp->next->key==key){
	 					temp->next->key = -1; //change the key to something we won't be able to access
						temp->next->vlen = 0;
	 					if(temp->next->next == 0){ //we are deleteing the last item	{
							temp->next = pFree; //point to the next block
							cout<<"deleted node key"<< key<<endl;
							return 1;
						}
						else{ // we are in the middle
							Node* temp_new = temp->next->next; //create a new node to point to where we want to change
							temp->next->next = 0; //change the deleted node to point to null
							temp->next = temp_new; //reassign the pointers
							temp_new = 0; //temp points to nothing
							delete temp_new; //clean up 
							cout<<"deleted node key"<< key<<endl;
							return 1;
						}	
	 					
	 			}
		//	char* temp_val = (char*) temp +16; this is where the value would be
			temp = temp->next;
			}
		}
	}
		//again when we break we have either found the key or we are at the end of the list
	cout<<"Key "<<key<<" not found for deletion.\n \n";
	return 0;
	
}
char* 	Linked_list::Lookup (int key)
{
	if(key<0) { cout<<"Please use a key 0 or greater \n"; return 0;}
	for(int i=0; i<tValue;i++){
		Node* temp =tiers[i];
    	if(temp!=0){
	 		while(temp->next!=0&& !is_same_tier(temp,tiers,i)){
	 			if(temp->key==key){
	 				char* temp_char = (char*)temp +8;
					return temp_char; //returns node
	 			}
		//	char* temp_val = (char*) temp +16; this is where the value would be
			temp = temp->next;
			}
		}
	}
	//again when we break we have either found the key or we are at the end of the list
	cout<<"key not avaliable"; //if we are at the end of the list and the key does not match what we want
	return 0; //return failure

}
void 	Linked_list::PrintList ()
{
	
	for(int i=0; i<tValue;i++){ //traverses through each tier
		Node* temp =tiers[i];
    	if(temp!=0){
			cout<< "tier" << i <<"----------------------"<<endl;
	 		while(temp->next!=0&& !is_same_tier(temp,tiers,i)){
		 	cout<< "New Node"<<endl;
			cout <<"key:"<< temp->key <<'\t'<<"value length:"<<temp->vlen<<endl<<endl;
			cout <<temp<<endl;
		//	char* temp_val = (char*) temp +16; this is where the value would be
			temp = temp->next;
			}
		}
		else
			cout<< "tier" << i << " doesn't have any nodes"<<endl<<endl;
	}
}

bool Linked_list::is_same_tier(Node* t, vector<Node*> vec_t,int tier){ //checks if it is the same tier
	for(int i=0; i<tValue;i++){
		if(t==vec_t[i] && i!=tier)
				return true;
	}
	return false;
}

bool Linked_list::is_same_tier1(Node* t, vector<Node*> vec_t){ //checks if it is the same tier
	for(int i=0; i<tValue;i++){
		if(t==vec_t[i])
				return true;
	}
	return false;
}

int Linked_list:: fix_last_node_tiers(){
     for(int i=0; i<tValue;i++){ //traverses through each tier
		Node* temp =tiers[i];
    	if(temp!=0){
	 		while(temp->next!=0 && !is_same_tier1(temp,tiers)){
			temp = temp->next;
			}
			if(i<tValue-1){
				if(tiers[i+1]==0)
					temp->next=pFree;
				else
					temp->next=tiers[i+1];
			}
			else
				temp->next=pFree;
		}
	}
}