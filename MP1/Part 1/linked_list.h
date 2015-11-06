#ifndef _linked_list_h_                   // include file only once
#define _linked_list_h_

using namespace std;

struct Node
{
	Node* next; //8 bytes
	int key, vlen; //4 bytes each
	char value[];
	
	Node(Node* n = 0, int k = 0,int v = 0) : next(n),key(k), vlen(v) {}
	
};


struct Linked_list
{
	Node* head;
	Node* pFree;
	Node* here;
	int b_length;
	int m_length;
	Linked_list(Node* h = 0,Node* p = 0, int l = 0) : head(h), pFree(p), b_length(l) {}
	void 	Init (int M, int b); // initializes the linked list, should be called once from the main
	void 	Destroy (); 		 //	destroys the linked list and cleans resources
	int 	Insert (int key,const char * value_ptr, int value_len); // inserts the key and copies the value to the payload
	int 	Delete (int key); 	 // delete the whole block containing that particular key. When multiple entries with the same key, delete only the first one	
	char* 	Lookup (int key);	 //	Looks up the first item with the given and returns a pointer to the value portion (the value length and the actual value) 
							//(the user can read or modify after obtaining the pointer)
	void 	PrintList ();		// prints the entire list by following the next pointers. Print the keys and the length	of the value
};


#endif 
