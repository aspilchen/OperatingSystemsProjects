/**
 * Contains shell history array and function.
 * Treats shell_history as a circular array of size HISTORY_DEPTH
 */

/**
 * Records command to history using shell_history array.
 * cmnd_string: Character array to be saved to history.
 * in_background: Boolean representing if command was executed as background process.
 */
void history_log(char *tokens[], int num_tokens,  _Bool in_background);

/**
 * Print history to screen.
 */
void history_show();

/**
 * Gets command from history corresponding to its command number.
 * cmd_number: Number of command in history according to the history_show function.
 */
char* history_get(int cmd_number);

/**
 * Get last command executed.
 */
char* history_get_last();
