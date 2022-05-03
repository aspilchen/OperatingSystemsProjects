#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "test_functions.h"
#include "list.h"

void test_List_createNode(int i){
	struct nodeStruct *node = NULL;
	node = List_createNode(i);
	assert(node != NULL);
	assert(node->item == i);
	assert(node->next == NULL);
	free(node);
}

void test_List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node, int n){
	if(headRef != NULL && node != NULL){
		struct nodeStruct **arr = List_toArray(*headRef, n); // create an array to preserve list order
		struct nodeStruct *iter = *headRef; // create a node at head to iterate over list
		List_insertHead(headRef, node); // insert node at head
		assert(*headRef == node); // test if node as been inserted at head
		assert((*headRef)->next == iter); // test if node has been linked to rest of list
		for(int i = 0; i < n; i++){ // ensure list order has been preserved after insertion
			assert(arr[i] == iter);
			iter = iter->next;
		}
		free(arr);
	}
	else{
		struct nodeStruct **tmp_head = headRef;
		struct nodeStruct *tmp_node = node;
		List_insertHead(headRef, node);
		assert(tmp_head == headRef);
		assert(tmp_node == node);
	}
}

void test_List_insertTail(struct nodeStruct **headRef, struct nodeStruct *node, int n){
	if(headRef != NULL && node != NULL){
		struct nodeStruct **arr = List_toArray(*headRef, n); // create an array to preserve list order
		struct nodeStruct *iter = *headRef; // create a node at head to iterate over list
		if(*headRef == NULL){
			List_insertTail(headRef, node); // insert node into list
			assert(*headRef == node); // test to ensure node is inserted at head if list is empty
		}
		else{
			List_insertTail(headRef, node); // insert node into list
			for(int i = 0; i < n; i++){ // test to ensure list order has been preserved after insertion
				assert(arr[i] == iter);
				iter = iter->next;
			}
			assert(iter == node); // test to ensure node is inserted at tail
		}
		free(arr);
	}
	else{ // test invalid list or node inputs (should do nothing, or crash if error occurs)
		struct nodeStruct **tmp_head = headRef;
		struct nodeStruct *tmp_node = node;
		List_insertTail(headRef, node);
		assert(tmp_head == headRef);
		assert(tmp_node == node);
	}
}

void test_List_countNodes(struct nodeStruct *head, int size){
	assert(List_countNodes(head) == size);
}

/*
 * Tests to be sure List_findNode returns the predicted result
 */

void test_List_findNode(struct nodeStruct *head, int item, struct nodeStruct *prediction){
	struct nodeStruct *node = NULL;
	node = List_findNode(head, item);
	assert(node == prediction);
}

void test_List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct **arr = List_toArray(*headRef, List_countNodes(*headRef)); // save list in array before sorting
	struct nodeStruct *iter = *headRef;
	size_t listSize = List_countNodes(*headRef);
	int i = 0;
	while(arr[i] != node && i < listSize){ // find node in arr
		i++;
	}
	if(node == *headRef){
		iter = iter->next;
	}
	List_deleteNode(headRef, node);
	// compare contents of list before and after delete to ensure order has not changed and no nodes have been lost or overwritten
	for(int j = 0; j < listSize; j++){
		if(i != j){
			assert(arr[j] == iter);
			iter = iter->next;
		}
	}
	assert(List_countNodes(*headRef) == listSize-1); // assure list has one less node after deletion
}

void test_List_sort(struct nodeStruct **headRef){
	struct nodeStruct **arr = List_toArray(*headRef, List_countNodes(*headRef)); // save list in array before sorting
	size_t listSize = List_countNodes(*headRef);
	struct nodeStruct *node;
	List_sort(headRef);
	node = *headRef;
	while(node != NULL && node->next != NULL){ // assure list is now sorted so n <= n->next
		assert(node->item <= node->next->item);
		node = node->next;
	}
	for(int i = 0; i < listSize; i++){ // compare contents of list before and after sorting to ensure no lost or overwritten nodes
		assert(List_findExactNode(*headRef, arr[i]) != NULL);
	}
	if(List_countNodes(*headRef) == 0){
		assert(*headRef == NULL);
	}
	free(arr);
}

void test_List_toArray(struct nodeStruct *head, int n){
	struct nodeStruct **arr = List_toArray(head, n); // create array from list
	struct nodeStruct *iter = head;
	for(int i = 0; i < n; i++){ // ensure list and array have the same nodes in correct order
		assert(arr[i] == iter);
		iter = iter->next;
	}
	free(arr);
}

struct nodeStruct* List_findExactNode(struct nodeStruct *head, struct nodeStruct *n){
	struct nodeStruct *iter = head;
	while(iter != n && iter != NULL){
		iter = iter->next;
	}
	return iter;
}
