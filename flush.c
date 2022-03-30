#include "parsing.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void generate_prompt(char *prmpt, size_t sz)
{
    bzero(prmpt, sz);
    getcwd(prmpt, sz);
    strcat(prmpt, ": ");
}

int main(int argc, char *argv[]) {
    char prmpt[1024];
    char buff[1024];
    int status;
    generate_prompt(prmpt, sizeof(prmpt));

    while ((status = get_line(prmpt, buff, sizeof(buff))) != NO_INPUT)
    {
        if (status == TOO_LONG) {
            printf("Too long\n");
        } else {
            char **args;
            unsigned int argc;
            parse_line(buff, strlen(buff), &args, &argc);
            for (int i = 0; i < argc; i++)
            {
                printf("%i, ", i);
                printf("%s\n", args[i]);
            }
        }
    }
    printf("exit\n");
    return 0;
}

