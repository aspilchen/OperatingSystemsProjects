#ifndef COMMAND_LENGTH
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10
#endif

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "../headers/built_in_commands.h"
#include "../headers/shell_history.h"
#include "../headers/interface_functions.h"
#include "../headers/signal_handler.h"

/**
 * Basic unix shell. This program will take user input from the command line and execute it as a shell command.
 * Attempting to open external commands will create new processes. You may choose to open these processes in 
 * the background if you wish to allow for continued use of the shell during the lifetime of the external process.
 * Any built in commands will be executed immediatly within the shell and will not create a new process.
 * Features included are basic history commands.
 */

int main(int argc, char* argv[])
{
	// Set signal handler for SIGINT
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT; // Function located in signal_handler.c
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);

	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];

	// Main loop
	while (true) {
		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().
		prompt_user();
		_Bool in_background = false;
		int num_tokens = read_command(input_buffer, tokens, &in_background);

		// Execute command if no SIGINT signal recieved
		if(errno != EINTR){
		 	exec_command(tokens, num_tokens, in_background);
		}
		
		// DEBUG: Dump out arguments
		//dump_arguments(input_buffer, tokens, &in_background);

		// Clean zombie processes
		while (waitpid(-1, NULL, WNOHANG) > 0);
	}
	return 0;
}
