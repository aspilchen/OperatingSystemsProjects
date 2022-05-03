#ifndef COMMAND_LENGTH
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include "../headers/built_in_commands.h"
#include "../headers/shell_history.h"
#include "../headers/interface_functions.h"

/**
 * Functions for interfacing with user. Execution of commands starts here.
 */


/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;
			// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
int read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
		int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);

if ( (length < 0) && (errno !=EINTR) ){
    perror("Unable to read command. Terminating.\n");
    exit(-1);  /* terminate with error */
}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return token_count;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
	return token_count;
}

/**
 * Forks a new external process and waits if in_background == false
 * tokens[]: Array of character arrays containing tokens from the tokenize function where token[0] is the new process
 *	to execute and the remaining are its arguments.
 * in_background: Boolean showing if new process to be run in background.
 */
void fork_process(char *tokens[], _Bool in_background)
{
	char fork_err_mssg[]  = "Unable to execute program.\n";
	char exec_err_mssg[] =  ": Unknown command.\n";
	// Fork
	pid_t var_pid = fork();
	int wstatus;

	// Print error message and return if fork fails.
	if (var_pid < 0) {
		write_string(fork_err_mssg);
		return;
	}
	// Execute new program in child process.
	else if (var_pid == 0) {
		int exec_success = execvp(tokens[0], tokens);
		// Print error message if execution fails.
		if (exec_success < 0){
			write_string(tokens[0]);
			write_string(exec_err_mssg);
		}
		// Exit child process
		exit(0);
	}
	// Parent waits if not in_background
	else if (!in_background) {
		waitpid(var_pid, &wstatus, 0);
	}
}

/**
 * Executes arguments as commands. Will attempt to fork and execute new process if command is not built in.
 * cmdn_string: Character array containing entire command string, used to save history.
 * tokens[]: Array of character arrays containing tokens from the tokenize function where token[0] is the new process
 *	to execute and the remaining are its arguments.
 * in_background: Boolean showing if new process to be run in background.
 */
void exec_command(char *tokens[], int num_tokens, _Bool in_background)
{
	_Bool log_cmd = true;

	// Return if tokens is empty.
	if(tokens[0] == NULL){
		return;
	}
	// Check for bang commands.
	else if (tokens[0][0] == '!') {
		log_cmd = false;
		if(tokens[0][1] == '!'){
			command_bangbang();
		}
		else{
			command_bang(&tokens[0][1]);
		}
	}

	// If bang was called, do not log command in this execution, return instead
	if(log_cmd){
		history_log(tokens, num_tokens, in_background);
	}
	else{
		return;
	}

	// Check for remaining built in commands.
	if (strcmp(tokens[0], "exit") == 0) {
		command_exit();
	}
	else if (strcmp(tokens[0], "pwd") == 0) {
		command_pwd();
	}
	else if (strcmp(tokens[0], "cd") == 0) {
		command_cd(tokens[1]);
	}
	else if (strcmp(tokens[0], "type") == 0) {
		command_type(tokens[1]);
	}
	else if (strcmp(tokens[0], "history") == 0) {
		command_history();
	}
	// Fork new process and use system call to execute
	else{
		fork_process(tokens, in_background);
	}
}

/**
 * Display user prompt.
 */
void prompt_user()
{
	command_pwd();
	write(STDOUT_FILENO, "$ ", strlen("$ "));
}

/**
 * Used for debugging purposes. Dumps all arguments read from command line.
 * tokens[]: Array of character arrays containing tokens from the tokenize function where token[0] is the new process
 *	to execute and the remaining are its arguments.
 * in_background: Boolean showing if new process to be run in background.
 */
void dump_arguments(char buffer[], char *tokens[], const _Bool *in_background)
{
	char str[] = "  Buffer:";
	char newline[] = "\n";
	for (int i = 0; tokens[i] != NULL; i++) {
		write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
		write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
	write_string(str);
	write_string(buffer);
	write_string(newline);
	if (in_background) {
		write(STDOUT_FILENO, "Run in background.\n", strlen("Run in background.\n"));
		}
}

/**
 * Writes string to STDOUT.
 * str[]: Character array containing string to write.
 */
void write_string(const char str[])
{
	write(STDOUT_FILENO, str, strlen(str));
}
