/*
 * I included test functions to allow for fast repeated testing.
 * This is probably a bit overkill but I don't have alot to do in the first week
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "test_functions.h"

int main(){
	struct nodeStruct *list_one = NULL;
	struct nodeStruct *list_two = NULL;
	struct nodeStruct *list_three = NULL;
	struct nodeStruct *list_four = NULL;
	struct nodeStruct *list_five = NULL;
	struct nodeStruct *list_six = NULL;


	printf("--- testing function List_createNode(int item) ---\n");
	for(int i = -100; i < 100; i++){
		test_List_createNode(i);
	}

	printf("--- testing function List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node) ---\n");
	for(int i = 0; i < 200; i++){
		struct nodeStruct *ll = NULL;
		struct nodeStruct *node = List_createNode(-(i-100));
		test_List_insertHead(NULL, node, 0);
		test_List_insertHead(&ll, NULL, 0);
		test_List_insertHead(&ll, node, 0);
		test_List_insertHead(&list_one, node, i);
	}

	printf("--- testing function List_insertTail(struct nodeStruct **headRef, struct nodeStruct *node) ---\n");
	for(int i = 0; i < 77; i++){
		struct nodeStruct *ll = NULL;
		struct nodeStruct *node = List_createNode(i-35);
		test_List_insertTail(NULL, node, 0);
		test_List_insertTail(&ll, NULL, 0);
		test_List_insertTail(&ll, node, 0);
		test_List_insertTail(&list_two, node, i);
	}
	
	printf("--- testing function List_countNodes(struct nodeStruct *head) ---\n");
	for(int i = 0; i < 99; i++){
		struct nodeStruct *node = List_createNode(-(i-48));
		test_List_countNodes(node, 1);
		test_List_countNodes(list_three, i);
		List_insertHead(&list_three, node);
	}
	
	printf("--- testing function List_findNode(struct nodeStruct *head) ---\n");
	// stores nodes used as predictions for search
	struct nodeStruct *prediction_1 = NULL;
	struct nodeStruct *prediction_2 = NULL;
	struct nodeStruct *prediction_3 = NULL;
	struct nodeStruct *prediction_4 = NULL;
	struct nodeStruct *prediction_5 = NULL;
	// stores predicted nodes item values (use this to save nodes with these vales)
	int node_item_1 = -20;
	int node_item_2 = 0;
	int node_item_3 = 15;
	int node_item_4 = 20;
	int node_item_5 = 30;
	for(int i = 0; i < 2; i++){ // repeat to ensure it selects first node when duplicate values are stored
		// saves the prediction for the first node matching each node_item variable
		// tests the findNode function matching each prediction with its search
		for(int j = 0; j < 66; j++){ 
			struct nodeStruct *node = List_createNode(i-30);
			if(i == node_item_1 && prediction_1 != NULL){
				prediction_1 = node;
			}
			if(i == node_item_2 && prediction_2 != NULL){
				prediction_2 = node;
			}
			if(i == node_item_3 && prediction_3 != NULL){
				prediction_3 = node;
			}
			if(i == node_item_4 && prediction_4 != NULL){
				prediction_4 = node;
			}
			if(i == node_item_5 && prediction_5 != NULL){
				prediction_5 = node;
			}
			test_List_findNode(list_four, node_item_1, prediction_1);
			test_List_findNode(list_four, node_item_2, prediction_2);
			test_List_findNode(list_four, node_item_3, prediction_3);
			test_List_findNode(list_four, node_item_4, prediction_4);
			test_List_findNode(list_four, node_item_5, prediction_5);
			List_insertTail(&list_four, node);
		}
	}

	printf("--- testing function List_toArray(struct nodeStruct *head, int n) ---\n");
	for(int i = 0; i < 47; i++){
		struct nodeStruct *node = List_createNode(-(i-25));
		test_List_toArray(list_five, i);
		List_insertHead(&list_five, node);
	}

	printf("--- testing function List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *node) ---\n");
	for(int i = 0; i < 30; i++){
		struct nodeStruct *n = List_createNode(i);
		List_insertTail(&list_six, n);
	}
	for(int i = 0; i < 30; i+=2){
		struct nodeStruct *n = List_findNode(list_six, i);
		test_List_deleteNode(&list_six, n);
	}
	for(int i = 30; i > 0; i--){
		struct nodeStruct *n = List_findNode(list_six, i);
		if(n != NULL){
		test_List_deleteNode(&list_six, n);
		}
	}
	assert(List_countNodes(list_six) == 0);
	assert(list_six == NULL);

	printf("--- testing function List_sort(struct nodeStruct **headRef) ---\n");
	test_List_sort(&list_one);
	test_List_sort(&list_two);
	test_List_sort(&list_three);
	test_List_sort(&list_four);
	test_List_sort(&list_five);
	test_List_sort(&list_six);
	List_insertTail(&list_one, list_two);
	List_insertTail(&list_one, list_three);
	List_insertTail(&list_one, list_four);
	List_insertTail(&list_one, list_five);
	List_insertTail(&list_one, list_six);
	test_List_sort(&list_one);

	List_destroy(&list_one);

	printf("all tests passed\n");

	return EXIT_SUCCESS;
}
