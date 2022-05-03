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
int tokenize_command(char *buff, char *tokens[]);

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
int read_command(char *buff, char *tokens[], _Bool *in_background);

/**
 * Forks a new external process and waits if in_background == false
 * tokens[]: Array of character arrays containing tokens from the tokenize function where token[0] is the new process
 *	to execute and the remaining are its arguments.
 * in_background: Boolean showing if new process to be run in background.
 */
void fork_process(char *tokens[], _Bool in_background);

/**
 * Executes arguments as commands. Will attempt to fork and execute new process if command is not built in.
 * cmdn_string: Character array containing entire command string, used to save history.
 * tokens[]: Array of character arrays containing tokens from the tokenize function where token[0] is the new process
 *	to execute and the remaining are its arguments.
 * in_background: Boolean showing if new process to be run in background.
 */
void exec_command(char *tokens[], int num_tokens, _Bool in_background);

/**
 * Display user prompt.
 */
void prompt_user();

/**
 * Used for debugging purposes. Dumps all arguments read from command line.
 * tokens[]: Array of character arrays containing tokens from the tokenize function where token[0] is the new process
 *	to execute and the remaining are its arguments.
 * in_background: Boolean showing if new process to be run in background.
 */
void dump_arguments(char buffer[], char *tokens[], const _Bool *in_background);

/**
 * Writes string to STDOUT.
 * str[]: Character array containing string to write.
 */
void write_string(const char str[]);
