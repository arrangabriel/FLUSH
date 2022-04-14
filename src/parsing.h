#ifndef ____PARSING___H___
#define ____PARSING___H___

#include "command.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2

/**
 * @brief Get a string from stdin with the given prompt
 *
 * @param prmpt Prompt for the user.
 * @param buff Input buffer.
 * @param sz Size of input buffer.
 * @return Status
 */
int get_line(char *prmpt, char *buff, size_t sz);

/**
 * @brief Parse a command string into a Command struct
 * 
 * @param command_str Command string to parse
 * @param command Command struct to populate
 * @param bg background flag
 * 
 * @return Status
 */
int parse_command(char *command_str, Command *command, int bg);

/**
 * @brief Parse a string into command structs
 * 
 * @param line String to parse
 * @param commands Array of commands to populate
 * @param commandc Number of populated commands
 * @param bg Whether to run in background
 * 
 * @return Status
 */
int parse_line(char *line, Command *commands[], unsigned int *commandc, int *bg);

#endif
