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
    int bg;
    pid_t pid;
} Command;

Command *command_init(size_t len);

int command_del(Command *cmd);

#endif