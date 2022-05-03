#ifndef TEST_FUNCTIONS
#define TEST_FUNCTIONS
#include "list.h"

void test_List_createNode(int i);

void test_List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node, int n);

void test_List_insertTail(struct nodeStruct **headRef, struct nodeStruct *node, int n);

void test_List_countNodes(struct nodeStruct *head, int size);

void test_List_findNode(struct nodeStruct *head, int item, struct nodeStruct *prediction);

void test_List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *node);

void test_List_sort(struct nodeStruct **headRef);

void test_List_toArray(struct nodeStruct *head, int n);

void test_my_merge_sort(struct nodeStruct **arr, int left, int right);

void test_my_merge(struct nodeStruct **arr, int left, int mid, int right);

void test_List_destroy(struct nodeStruct **headRef);

void test_List_print(struct nodeStruct* head);

struct nodeStruct* List_findExactNode(struct nodeStruct *head, struct nodeStruct *n);


#endif
