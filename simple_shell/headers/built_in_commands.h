/**
 * All built in shell commands. These will not directly execute new processes and will
 * be performed within the shell itself. However bang commands may create child processes
 * if corresponding commands in history did so.
 */

/**
 * Exits shell with exit status 0.
 */
void command_exit();

/**
 * Displays present working directory.
 */
void command_pwd();

/**
 * Change working directory.
 * path: Character array containing string designating desired new directory.
 */
void command_cd(const char *path);

/**
 * Display if command is built in or external.
 * token: Character array containing string of command to check.
 */
void command_type(const char *token);

/**
 * Show last 10 commands entered from oldest to newest.
 */
void command_history();

/**
 * Execute command from history.
 * number_string: Character array containing an integer string.
 */
void command_bang(char *number_string);

/**
 * Re-executes last command entered.
 */
void command_bangbang();
