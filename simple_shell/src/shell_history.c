#ifndef COMMAND_LENGTH
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10
#endif

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/shell_history.h"
#include "../headers/interface_functions.h"

/**
 * Contains shell history array and function.
 * Treats shell_history as a circular array of size HISTORY_DEPTH
 */


// Circular array to contain history of shell commands.
char shell_history[HISTORY_DEPTH][COMMAND_LENGTH];
// Current command number.
int curr_command = 0;

/**
 * Records command to history using shell_history array.
 * cmnd_string: Character array to be saved to history.
 * in_background: Boolean representing if command was executed as background process.
 */
void history_log(char *tokens[], int num_tokens, _Bool in_background)
{
	// Get proper index for shell_history.
	int i = curr_command % HISTORY_DEPTH;

	if(in_background){
		num_tokens--;
	}

	// Copy cmnd_string into history.
	strcpy(shell_history[i], tokens[0]);
	for(int j = 1; j < num_tokens; j++){
		strcat(shell_history[i], " ");
		strcat(shell_history[i], tokens[j]);
	}

	// Add '&' if in_background is true.
	if(in_background){
		strcat(shell_history[i], " &");
	}

	// Ensure string saved is null terminated.
	//strcat(shell_history[i], "\0");

	curr_command++;
}

/**
 * Print history to screen.
 */
void history_show()
{
	int i = curr_command % HISTORY_DEPTH;
	int num_cmd = curr_command - HISTORY_DEPTH + 1; // Command number printed to screen.

	// Print contents of shell_history array from oldest command to newest command.
	while(num_cmd <= curr_command){
		if(num_cmd > 0){
			char str[COMMAND_LENGTH];
			// Format string to be printed.
			sprintf(str, "%i\t%s\n", num_cmd, shell_history[i]);
			write_string(str);
		}
		num_cmd++;
		i = (i + 1) % HISTORY_DEPTH;
	}
}

/**
 * Gets command from history corresponding to its command number.
 * cmd_number: Number of command in history according to the history_show function.
 */
char* history_get(int cmnd_number)
{
	char err_str[] = "SHELL: Event not found.\n";
	int oldest_cmnd = curr_command - HISTORY_DEPTH;

	// If cmnd_number within bounds, return appropriate string from history.
	if(cmnd_number <= curr_command && cmnd_number > oldest_cmnd){
		int i = (cmnd_number - 1) % HISTORY_DEPTH;
		return shell_history[i];
	}
	// If cmnd_number out of bounds, print error message and return null.
	else{
		write_string(err_str);
		return NULL;
	}
}

/**
 * Get last command executed.
 */
char* history_get_last()
{
	int i = (curr_command - 1) % HISTORY_DEPTH;
	return shell_history[i];
}
