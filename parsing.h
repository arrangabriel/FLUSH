#ifndef ____PARSING___H___
#define ____PARSING___H___

#include <stddef.h>
#include "command.h"

#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2

/**
 * @brief Get a string from stdin with the given prompt
 *
 * @param prmpt - Prompt for the user.
 * @param buff - Input buffer.
 * @param sz - Size of input buffer.
 * @return Status
 */
int get_line(char *prmpt, char *buff, size_t sz);

//unsigned int parse(char *line, size_t sz, char ***args, const char delim[]);

int parse_command(char *command_str, Command *command);

int parse_line(char *line, Command *commands[], unsigned int* commandc);

#endif
