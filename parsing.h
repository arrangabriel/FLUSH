#ifndef ____PARSING___H___
#define ____PARSING___H___

#include <stddef.h>
#include <sys/types.h>

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
 * @brief Splits a string by spaces and tabs and places each token into args, NULL-terminated.
 *
 * @param line - String to be split.
 * @param sz - Size of string.
 * @param args - Reference to string array to be modified.
 * @return The number of elements in args.
 */
unsigned int parse(char *line, size_t sz, char ***args, const char delim[]);

int parse_command(char *command_str, Command *command);

typedef struct Command
{
    char **args;
    unsigned int argc;
    int *input_redirects;
    int *output_redirects;
    pid_t pid;
} Command;

Command* command_init();

int command_del(Command *cmd);

#endif