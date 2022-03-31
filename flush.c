#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024

void sig_handler(int signo)
{
    // if (signo == SIGINT)
    //     printf("received SIGINT\n");
}

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
        if (!argc)
            chdir(getenv("HOME"));
        else if (chdir(args[1]))
            printf("'%s' - not a directory\n", args[1]);
    }
    else
    {
        // TODO, search for program in path and return status before forking.
        pid_t pid = fork();
        if (!pid)
        {
            // child
            if (!argc)
                execlp(args[0], args[0], NULL);
            else
                execvp(args[0], &args[0]);

            // printf("Command - %s not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
        // parent
        // TODO - figure out which events to wait on.
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            // Child process called exit
            // Return exit-status of child process
            return WEXITSTATUS(status);
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    char prmpt[MAX_LENGTH];
    char buff[MAX_LENGTH];
    int status;
    generate_prompt(prmpt, sizeof(prmpt));

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    while ((status = get_line(prmpt, buff, sizeof(buff))) != NO_INPUT)
    {
        if (status == TOO_LONG)
        {
            printf("Too long\n");
        }
        else
        {
            if (strlen(buff) == 0)
                continue;

            char **args = (char **)malloc((strlen(buff) / 2) + 2);
            unsigned int argc = parse_line(buff, strlen(buff), &args);
            int found;
            int status = run_args(args, argc - 1);
            generate_prompt(prmpt, sizeof(prmpt));
            printf("Exit status [");
            for (int i = 0; i < argc; i++)
            {
                printf("%s", args[i]);
                if (i != argc - 1)
                {
                    printf(" ");
                }
            }
            printf("] = %i\n", status);
            free(args);
        }
    }
    printf("exit\n");
    exit(EXIT_SUCCESS);
}
