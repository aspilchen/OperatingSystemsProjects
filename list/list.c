#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
 struct nodeStruct* List_createNode(int item){
	 struct nodeStruct *node = malloc(sizeof(struct nodeStruct));
	 node->item = item;
	 node->next = NULL;
	 return node;
}

/*
 * Insert node at the head of the list.
 */
void List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node){
	if(headRef == NULL || node == NULL){
		return;
	}
	else if(*headRef != NULL){
		node->next = *headRef;
		*headRef = node;
	}
	else if(*headRef == NULL){
		*headRef = node;
	}
}

/*
 * Insert node after the tail of the list.
 */
void List_insertTail(struct nodeStruct **headRef, struct nodeStruct *node){
	if(headRef == NULL || node == NULL){
		return;
	}
	else if(*headRef == NULL){
		*headRef = node;
	}
	else{
		struct nodeStruct *iter = *headRef;
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = node;
	}
}

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes(struct nodeStruct *head){
		size_t i = 0;
		struct nodeStruct *iter = head;
		while(iter != NULL){
			i++;
			iter = iter->next;
		}
		return i;
}

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int item){
	struct nodeStruct *iter = head;
	while(iter != NULL && iter->item != item){
		iter = iter->next;
	}
	return iter;
}

/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set to a valid node 
 * in the list. For example, the client code may have found it by calling 
 * List_findNode(). If the list contains only one node, the head of the list 
 * should be set to NULL.
 */
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node){
	if(headRef != NULL && *headRef != NULL){
		struct nodeStruct *iter, *tmp;
		iter = (*headRef);
		if(*headRef == node){ // check head
			*headRef = (*headRef)->next;
			free(node);
		}
		while(iter != NULL){
			if(iter->next == node){
				tmp = iter;
				iter = iter->next;
				tmp->next = iter->next;
				free(iter);
				return;
			}
			else{
				iter = iter->next;
			}
		}
	}
}

/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef){
	if(headRef != NULL && *headRef != NULL){
		size_t size = List_countNodes(*headRef);
		struct nodeStruct **arr = List_toArray(*headRef, size);
		my_merge_sort(arr, 0, size-1);
		for(int i = 0; i < size-1; i++){
			arr[i]->next = arr[i+1];
			*headRef = arr[0];
			arr[size-1]->next = NULL;
		}
		free(arr);
	}
}

/*
 * Returns an array of nodeStruct pointers from a linked list
 */
struct nodeStruct** List_toArray(struct nodeStruct *head, const int n){
	struct nodeStruct **arr = malloc(sizeof(head) * n);
	struct nodeStruct *iter = head;
	int i = 0;
	while(iter != NULL){
		arr[i] = iter;
		iter = iter->next;
		i++;
	}
	return arr;
}

/*
 * Merge sort an array of nodeStruct pointers
 */
void my_merge_sort(struct nodeStruct **arr, int left, int right){
	if(left < right){
		int mid = (left + right)/2;
		my_merge_sort(arr, left, mid);
		my_merge_sort(arr, mid+1, right);
		my_merge(arr, left, mid, right);
	}
}

/*
 * Merge arrays of nodeStruct pointers
 * used for my_merge_sort
 */
void my_merge(struct nodeStruct **arr, int left, int mid, int right){
	int i, j, k;
	i = j = 0;
	int left_size = mid - left + 1;
	int right_size = right - mid;
	struct nodeStruct *left_arr[left_size];
	struct nodeStruct *right_arr[right_size];
	for(i = 0; i < left_size; i++){
		left_arr[i] = arr[i+left];
	}
	for(j = 0; j < right_size; j++){
		right_arr[j] = arr[j+mid+1];
	}
	i = j = 0;
	k = left;
	while(i < left_size && j < right_size){
		if(left_arr[i]->item < right_arr[j]->item){
			arr[k] = left_arr[i];
			i++;
		}
		else{
			arr[k] = right_arr[j];
			j++;
		}
		k++;
	}
	while(i < left_size){
		arr[k] = left_arr[i];
		i++;
		k++;
	}
	while(j < right_size){
		arr[k] = right_arr[j];
		j++;
		k++;
	}
}

/*
 * Destroys entire list
 */
void List_destroy(struct nodeStruct **headRef){
	if(headRef != NULL && *headRef != NULL){
		struct nodeStruct *iter, *tracer;
		iter = (*headRef)->next;
		tracer = *headRef;
		while(iter != NULL){
			free(tracer);
			tracer = iter;
			iter = iter->next;
		}
		free(tracer);
		*headRef = NULL;
	}
}

/*
 * Print list vertically
 */
void List_print(struct nodeStruct *head){
	struct nodeStruct* iter = head;
	while(iter != NULL){
		printf("%i\n", iter->item);
		iter = iter->next;
	}
}










