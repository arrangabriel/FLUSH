#ifndef ____PARSING___H___
#define ____PARSING___H___

#include <stddef.h>

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
/**
 * @brief Splits a string by spaces and tabs and places each token into args.
 *
 * @param line - String to be split.
 * @param sz - Size of string.
 * @param args - Reference to string array to be modified.
 * @return The number of elements in args.
 */
unsigned int parse_line(char *line, size_t sz, char ***args);

#endif