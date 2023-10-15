#include<stdio.h>




struct Node
{
	int priority;
	struct Node* next;
};

void __Add_node_in_list( struct Node* node , struct Node* next , struct Node* prev )
{
	node->next = next;

	if( prev  )
	{
		prev->next = node;
	}
	else
	{
		root = node;
	}
}

void Print(struct Node* node)
{
	struct Node* curr = node;
	while( curr )
	{
		printf("Node(%p) [%d] \n",curr,curr->priority);
		curr = curr->next;
	}
}

void Add( struct Node* node , struct Node* root )
{
	node->next = NULL;
	if( *root == NULL )
	{
		*root = node;
	}
	else
	{
		struct Node* cur  = root;
		struct Node* prv  = NULL;
		struct Node* next = cur->next;

		while( cur )
		{
			if( cur->priority >= node->priority )
			{
				__Add_node_in_list(node,cur,prv);
				break;
			}
			prv = curr;
			cur = cur->next;
			next = cur->next;
		} 
	}
}

void Remove( struct Node* node , struct Node* root )
{

}

void Reverse( struct Node* root )
{

}


int main()
{



}