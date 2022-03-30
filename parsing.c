#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parsing.h"

int get_line(char *prmpt, char *buff, size_t sz)
{
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int parse_line(char *line, size_t sz, char ***args, unsigned int *argc)
{
    char delim[] = " \t";
    char *token;
    *argc = 0;
    *args = (char **)malloc(strlen(line) / 2 + 1);
    token = strtok(line, delim);
    while (token != NULL)
    {
        (*argc)++;
        // printf("%i\n", *argc);
        char arg[strlen(token)];
        strcpy(arg, token);
        printf("hei\n");
        *args[*argc] = arg;
        printf("hei\n");
        token = strtok(NULL, delim);
    }
    for (int i = 0; i < *argc; i++)
    {
        // printf("%i\n", i);
        printf("%i\n", *args[i]);
    }
    return 0;
}
