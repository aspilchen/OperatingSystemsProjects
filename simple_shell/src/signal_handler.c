#include "../headers/signal_handler.h"
#include "../headers/built_in_commands.h"
#include <unistd.h>

/**
 * Functions to be executed when shell recieves a shignal.
 */

/**
 * Dispays command history.
 */
void handle_SIGINT()
{
	write(STDOUT_FILENO, "\n", 1);
	command_history();
}
