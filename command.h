#ifndef __COMMAND__H__
#define __COMMAND__H__

#include <sys/types.h>

typedef struct Command
{
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