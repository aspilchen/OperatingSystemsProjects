#include "mystring.h"
#include <assert.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * MY TEST FUNCTIONS! THESE ARE USED FOR TESTING PURPOSES ONLY!
 */

void test_mystrlen(const char* s){
	int actual_val = strlen(s);
	int myval = mystrlen(s);
	assert(myval == actual_val);
}

void test_mystrcpy(const char* src){
	char myval[100];
	char* mypntr = NULL;
	mypntr = mystrcpy(myval, src);
	assert(strcmp(myval, src) == 0);
	assert(mypntr == myval);
}

void test_mystrcmp(const char* s1, const char* s2){
	int myval = mystrcmp(s1, s2);
	int actual_val = strcmp(s1, s2);
	int rounded_val = 0;
	if(actual_val < 0){
		rounded_val = -1;
	}
	else if(actual_val > 0){
		rounded_val = 1;
	}
	assert(myval == rounded_val);
}

void test_mystrdup(const char* s1){
	char *dupStr = mystrdup(s1);
	assert(!strcmp(s1, dupStr));
	assert(s1 != dupStr);
	if (dupStr != NULL)
		free(dupStr);
}

/*
 * The following are simple tests to help you.
 * You should develop more extensive tests to try and find bugs.
 */
int main()
{
	printf("\n--- Testing function mystrlen(const char * str) ---\n");
	test_mystrlen(" ");
	test_mystrlen("cmpt 300");
	test_mystrlen("qwertyuiopasdfghjklzxcvbnm,");
	test_mystrlen(",./;:'\"[]}{");
	test_mystrlen("ThIs iS aNoThEr TesT");
	test_mystrlen("");

	printf("\n--- Testing function mystrcpy(const char* dst, const char* src) ---\n");
	test_mystrcpy("This is a test");
	test_mystrcpy("");
	test_mystrcpy(" ");
	test_mystrcpy("I dOnT kNoW wHaT eLsE tO tEsT hErE");
	test_mystrcpy("123456");

	printf("\n--- Testing function mystrcmp(const char* s1, const char* s2) ---\n");
	test_mystrcmp("I love coding", "I love coding");
	test_mystrcmp("i love coding", "I love coding");
	test_mystrcmp("I love coding", "I love codin");
	test_mystrcmp("I ove coding", "I love coding");
	test_mystrcmp("I love codin", "I love coding");
	test_mystrcmp("123456", "123456");
	test_mystrcmp("abcdefg", "bcdefgh");
	test_mystrcmp( "bcdefgh", "abcdefg");
	test_mystrcmp( " ", "abcdefg");
	test_mystrcmp( "", "abcdefg");
	test_mystrcmp( "bcdefgh", "");
	test_mystrcmp( "", "");

	printf("\n--- Testing function mystrdup(const char* s1) ---\n");
	test_mystrdup("This is a test");
	test_mystrdup("ThIs Is a TeSt");
	test_mystrdup("123456789][;l.,");
	test_mystrdup(" ");
	test_mystrdup("");

	printf ("\nall tests passed\n");

	return 0;
}
