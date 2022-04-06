#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parsing.h"

// TODO - this whole function can be replaced by a call to getline (from stdio)
int get_line(char *prmpt, char *buff, size_t sz)
{
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL)
    {
        printf("%s", prmpt);
        fflush(stdout);
    }
    if (fgets(buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff) - 1] != '\n')
    {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff) - 1] = '\0';
    return OK;
}

unsigned int parse(char *line, size_t sz, char ***args, const char delim[])
{
    char *token;
    unsigned int argc = 0;
    token = strtok(line, delim);
    while (token != NULL)
    {
        argc++;
        (*args)[argc - 1] = token;
        token = strtok(NULL, delim);
    }
    (*args)[argc] = NULL;
    return argc;
}

Command *command_init()
{
    Command *cmd = (Command *)malloc(sizeof(Command));
    // Error handling?
    cmd->argc = 0;
    cmd->pid = 0;
    return cmd;
}

int command_del(Command *cmd)
{
    free(cmd);
    // Error handling?
    return 0;
}

int parse_command(char *command_str, Command *command)
{
    char *token;
}

int parse_line(char *line, size_t sz, Command *commands[])
{
    char *command_str;
    while ((command_str = strsep(line, "|")) == NULL)
    {
        Command *command = command_init();
    }
}