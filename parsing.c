#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parsing.h"
#include "command.h"

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

int parse_command(char *command_str, Command *command)
{
    char *arg;
    // This change may be wrong, but I dont think so
    // char **space_sep = (char **)malloc(((strlen(command_str) / 2) + 1));
    // okay, this fixed it, wtf
    char **space_sep = (char **)malloc(((strlen(command_str) / 2) + 1) * sizeof(char *));

    int i = 0;
    while ((arg = strsep(&command_str, " \t")) != NULL)
    {
        space_sep[i] = arg;
        i++;
    }

    for (int j = 0; j < i; j++)
    {
        if (strcmp(space_sep[j], "<") == 0)
        {
            command->input_redirect = space_sep[++j];
        }
        else if (strcmp(space_sep[j], ">") == 0)
        {
            command->output_redirects[(command->outc)++] = space_sep[++j];
        }
        else if (strcmp(space_sep[j], "&") == 0)
        {
            if (j != (i - 1))
                return -1;

            command->bg = 1;
        }
        else
        {
            command->args[(command->argc)++] = space_sep[j];
        }
    }
    command->args[(command->argc) + 1] = NULL;
}

int parse_line(char *line, Command *commands[], int *commandc)
{
    char *command_str;
    (*commandc) = 0;
    while ((command_str = strsep(&line, "|")) != NULL)
    {
        Command *command = command_init(strlen(command_str));
        parse_command(command_str, command);
        commands[(*commandc)++] = command;
    }
}
