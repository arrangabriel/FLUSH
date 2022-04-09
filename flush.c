#include "parsing.h"
#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_LENGTH 1024
#define TEMP "/tmp/flush_out"

#define RESET "\x1B[0m"
#define KREDB "\x1B[31;1m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KCYNB "\x1B[36;1m"
#define KBLU "\x1B[34m"
#define KWHT "\x1B[37m"

extern int errno;

void sig_handler(int signo) {}

int flush_cd(char **args, unsigned int argc);

char *builtin_str[] = {
    "cd"
    //"help",
    //"exit",
};

int (*builtin_func[])(char **, unsigned int) = {
    &flush_cd
    //&flush_exit,
};

size_t flush_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int flush_cd(char **args, unsigned int argc)
{
    if (argc == 1)
        chdir(getenv("HOME"));
    else if (chdir(args[1]))
    {
        printf(KYEL "Not a directory: " RESET KCYNB "%s" RESET "\n", args[1]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void generate_prompt(char *prmpt, size_t sz)
{
    bzero(prmpt, sz);
    char path[PATH_MAX];
    char end[] = RESET KWHT ": " RESET;
    getcwd(path, PATH_MAX);
    strcpy(prmpt, KBLU);
    strncat(prmpt, path, sz - strlen(end));
    strcat(prmpt, end);
}

int run_command(Command *runcommand, Command *outcommand)
{
    for (int i = 0; i < flush_num_builtins(); i++)
    {
        if (strcmp((runcommand->args)[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(runcommand->args, runcommand->argc);
    }

    int pid = fork();

    if (pid)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);

        printf("Process return status %i\n", status);
        return EXIT_FAILURE;
    }

    int std_out = dup(1);
    // In child
    if (strcmp(runcommand->input_redirect, "") != 0)
    {
        int fd;
        if ((fd = open(runcommand->input_redirect, O_RDONLY)) < 0)
        {
            // Not exhaustive error handling
            printf(KYEL "Input file: " KCYNB "%s" RESET KYEL " not found\n" RESET, runcommand->input_redirect);
            exit(EXIT_FAILURE);
        }
        dup2(fd, 0);
    }

    if (strcmp(runcommand->output_redirect, "") != 0)
    {
        int fd;
        if ((fd = open(runcommand->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) < 0)
        {
            // Not exhaustive error handling
            printf(KYEL "Output file: " KCYNB "%s" RESET KYEL " could not be created\n" RESET, runcommand->output_redirect);
            exit(EXIT_FAILURE);
        }
        dup2(fd, 1);
    }

    // piping setup
    if (outcommand == NULL)
    {
    }
    else
    {
    }

    execvp((runcommand->args)[0], &(runcommand->args)[0]);

    dup2(std_out, 1);

    switch (errno)
    {
    case EACCES:
        perror(KREDB "Permission denied" RESET "\n");
        break;
    case ENOENT:
        perror(KREDB "Command not found" RESET "\n");
        break;
    default:
        perror(KREDB "Unknown error" RESET "\n");
        break;
    }
    exit(1);
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
            printf("Too long\n");
        else
        {
            if (strlen(buff) == 0)
                continue;

            Command *commands[(strlen(buff) + 1)];
            bzero(commands, sizeof(commands)); // = (Command **)malloc((strlen(buff) + 1) * sizeof(Command *));
            unsigned int commandc = 0;

            status = parse_line(buff, commands, &commandc);

            for (int i = 0; i < commandc; i++)
            {
                if (i < (commandc - 1))
                    status = run_command(commands[i], commands[i + 1]);
                else
                    status = run_command(commands[i], NULL);
            }

            generate_prompt(prmpt, sizeof(prmpt));

            char *status_color = (status) ? RESET KREDB : RESET KGRN;
            printf(RESET KYEL "Exit status " RESET KCYNB "[");
            for (int i = 0; i < commandc; i++)
                printf("%s", commands[i]->cmd_str);
            printf("]%s = %i\n" RESET, status_color, status);
            for (int i = 0; i < commandc; i++)
                command_del(commands[i]);
        }
    }
    printf(RESET KGRN "exit" RESET "\n");
    exit(EXIT_SUCCESS);
}
