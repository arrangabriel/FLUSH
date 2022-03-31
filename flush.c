#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LENGTH 1024

void generate_prompt(char *prmpt, size_t sz)
{
    bzero(prmpt, sz);
    getcwd(prmpt, sz);
    strcat(prmpt, ": ");
}

int run_args(char **args, unsigned int argc)
{
    if (strcmp(args[0], "cd") == 0)
    {
        printf("CD - ran.\n");
    }
    else
    {
        printf("%s - ran.\n", args[0]);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char prmpt[MAX_LENGTH];
    char buff[MAX_LENGTH];
    int status;
    generate_prompt(prmpt, sizeof(prmpt));

    while ((status = get_line(prmpt, buff, sizeof(buff))) != NO_INPUT)
    {
        if (status == TOO_LONG)
        {
            printf("Too long\n");
        }
        else
        {
            char **args = (char **)malloc((strlen(buff) / 2) + 1);
            unsigned int argc = parse_line(buff, strlen(buff), &args);
            run_args(args, argc);
            free(args);
        }
    }
    printf("exit\n");
    return EXIT_SUCCESS;
}
