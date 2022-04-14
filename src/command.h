#ifndef __COMMAND__H__
#define __COMMAND__H__

#include <sys/types.h>
#include <stdlib.h>

typedef struct Command
{
    char *cmd_str;
    char **args;
    unsigned int argc;
    char *input_redirect;
    char *output_redirect;
    int input_pipe;
    int output_pipe;
    int bg;
    pid_t pid;
} Command;

/**
 * @brief Initialize a Command struct
 *
 * @param len Length of the command string
 * @return Command* Pointer to the initialized Command struct
 */
Command *command_init(size_t len);

/**
 * @brief Free a Command struct
 *
 * @param cmd Command struct to free
 * @return Status
 */
int command_del(Command *cmd);

#endif