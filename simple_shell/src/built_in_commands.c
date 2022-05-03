#ifndef COMMAND_LENGTH
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../headers/built_in_commands.h"
#include "../headers/shell_history.h"
#include "../headers/interface_functions.h"

/**
 * All built in shell commands. These will not directly execute new processes and will
 * be performed within the shell itself. However bang commands may create child processes
 * if corresponding commands in history did so.
 */


/**
 * Exits shell with exit status 0.
 */
void command_exit()
{
	exit(0);
}

/**
 * Displays present working directory.
 */
void command_pwd()
{
	char curr_dir[COMMAND_LENGTH];
	getcwd(curr_dir, COMMAND_LENGTH);
	write(STDOUT_FILENO, curr_dir, strlen(curr_dir));
}

/**
 * Change working directory.
 * path: Character array containing string designating desired new directory.
 */
void command_cd(const char *path)
{
	char err_mssg[] =  "Invalid directory.\n";

	// Display error message if cwd failed.
	if(chdir(path) != 0){
		write_string(err_mssg);
	}
}

/**
 * Display if command is built in or external.
 * token: Character array containing string of command to check.
 */
void command_type(const char *token)
{
	char err_mssg[] = "Error: missing second argument. Command skipped.\n";
	char built_in_string[] = " is a shell300 builtin\n";
	char external_string[] = " is external to shell300\n";

	// Display error message if invalid argument.
	if(token == NULL){
		write_string(err_mssg);
		return;
	}
	// Display token and build_in_string if token is build in command.
	else if(strcmp(token, "exit") == 0 ||
			strcmp(token, "pwd") == 0 ||
			strcmp(token, "cd") == 0 ||
			strcmp(token, "type") == 0)
	{
		write_string(token);
		write_string(built_in_string);
	}
	// Token is not built in command, display token and external_string.
	else{
		write_string(token);
		write_string(external_string);
	}
}

/**
 * Show last 10 commands entered from oldest to newest.
 */
void command_history()
{
	history_show();
}

/**
 * Execute command from history.
 * number_string: Character array containing an integer string.
 */
void command_bang(char *number_string)
{
	char err_mssg[] =  "SHELL: Unknown history command.\n";
	char newline[] = "\n";
	int cmd_number = atoi(number_string);

	// Display error message if invalid argument.
	if(cmd_number == 0){
		write_string(err_mssg);
	}

	// Get command from history.
	char *buffer = history_get(cmd_number);
	char *tokens[NUM_TOKENS];
	_Bool in_background = false;
	int token_count = 0;

	// Tokenize and execute command.
	if(buffer != NULL){
		write_string(buffer);
		write_string(newline);
		token_count = tokenize_command(buffer, tokens);
		// Extract if running in background:
		if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
			in_background = true;
			tokens[token_count - 1] = 0;
		}
		exec_command(tokens, token_count, in_background);
	}
}

/**
 * Re-executes last command entered.
 */
void command_bangbang()
{
	char newline[] = "\n";
	// Get command from history.
	char *buffer = history_get_last();
	char *tokens[NUM_TOKENS];
	_Bool in_background = false;
	int token_count = 0;

	// Tokenize and execute command.
	if(buffer != NULL){
		write_string(buffer);
		write_string(newline);
		token_count = tokenize_command(buffer, tokens);
	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		in_background = true;
		tokens[token_count - 1] = 0;
	}
		exec_command(tokens, token_count, in_background);
	}
}
