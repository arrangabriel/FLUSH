#ifndef __COMMAND__H__
#define __COMMAND__H__

#include <sys/types.h>
#include <stdlib.h>

typedef struct Command
{
    // TODO - add field for original string, we lose position of redirects as is
    char **args;
    unsigned int argc;
    char *input_redirect;
    char **output_redirects;
    unsigned int outc;
    int bg;
    pid_t pid;
} Command;

Command *command_init(size_t len);

int command_del(Command *cmd);

#endif