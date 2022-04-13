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

char *trimwhitespace(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    return str;
}

int parse_command(char *command_str, Command *command, int bg)
{
    // TODO - investigate bug when parsing with spaces in quotes
    char *arg;
    char *space_sep[(strlen(command_str) / 2) + 1];

    command_str = trimwhitespace(command_str);
    (command->cmd_str) = (char *)malloc(strlen(command_str) * sizeof(char));
    strcpy(command->cmd_str, command_str);
    command->bg = bg;

    int i = 0;
    while ((arg = strsep(&command_str, " \t")) != NULL)
    {
        if (strcmp(arg, "") != 0)
        {
            printf("%i\n", i);
            space_sep[i] = arg;
            i++;
        }
    }

    for (int j = 0; j < i; j++)
    {
        if (strcmp(space_sep[j], "<") == 0)
        {
            if (j == i)
                return EXIT_FAILURE;
            command->input_redirect = space_sep[++j];
        }
        else if (strcmp(space_sep[j], ">") == 0)
        {
            if (j == i)
                return EXIT_FAILURE;
            command->output_redirect = space_sep[++j];
        }
        else
            command->args[(command->argc)++] = space_sep[j];
    }
    command->args[(command->argc) + 1] = NULL;
    return EXIT_SUCCESS;
}

int parse_line(char *line, Command *commands[], unsigned int *commandc, int *bg)
{
    char *command_str;
    (*commandc) = 0;
    size_t line_len = strlen(line);

    if (strcmp(&line[line_len - 1], "&") == 0)
    {
        line[line_len - 2] = '\0';
        line_len -= 2;
        (*bg) = 1;
    }
    else
        (*bg) = 0;

    // fix segfault on single command with pipe, or single pipe only
    while ((command_str = strsep(&line, "|")) != NULL)
    {
        Command *command = command_init(strlen(command_str));
        if (parse_command(command_str, command, *bg))
        {
            // delete commands, then return failiure
            for (int i = 0; i < (*commandc); i++)
                command_del(commands[i]);
            return EXIT_FAILURE;
        }
        commands[(*commandc)++] = command;
    }
    return EXIT_SUCCESS;
}

