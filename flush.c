#include "parsing.h"
#include "command.h"
#include "list.h"
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

#define MAX_CMD_LEN 1024

#define RESET "\x1B[0m"
#define KREDB "\x1B[31;1m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KCYNB "\x1B[36;1m"
#define KBLU "\x1B[34m"
#define KWHT "\x1B[37m"

List *bg_jobs;

extern int errno;

void sig_handler(int signo) {}

int check_jobs();
int flush_cd(char **args, unsigned int argc);
int flush_exit(char **args, unsigned int argc);
int flush_jobs(char **args, unsigned int argc);

char *builtin_str[] = {
    "cd",
    "exit",
    //"help",
    "jobs",
};

int (*builtin_func[])(char **, unsigned int) = {
    &flush_cd,
    &flush_exit,
    &flush_jobs,
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

int flush_exit(char **args, unsigned int argc)
{
    exit(EXIT_SUCCESS);
}

int flush_jobs(char **args, unsigned int argc)
{
    // Do this to remedy showing finished jobs
    check_jobs();

    if (list_length(bg_jobs) == 0)
    {
        printf(KWHT "No background jobs.\n" RESET);
        return EXIT_SUCCESS;
    }
    printf(KWHT "Background jobs:\n" RESET);
    Node *current = bg_jobs->head;
    while (current != NULL)
    {
        printf(KWHT "PID: [%d] - [%s]\n" RESET, (current->cmd)->pid, (current->cmd)->cmd_str);
        current = current->next;
    }
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

int check_jobs()
{
    Node *current = bg_jobs->head;
    while (current != NULL)
    {
        Command *job = current->cmd;
        int status;
        if (waitpid(job->pid, &status, WNOHANG) == 0)
        {
            current = current->next;
            continue;
        }

        char *status_color = (status) ? RESET KREDB : RESET KGRN;
        printf(RESET KYEL "Exit status " RESET KCYNB "[");
        printf("%s", job->cmd_str);
        printf("]%s = %i\n" RESET, status_color, status);

        current = current->next;
        list_remove(bg_jobs, job);
        command_del(job);
    }
    return 0;
}

/**
 * @brief runs a command structure
 *
 * @param runcommand the command to run
 * @param outcommand the command to run if the command is a pipe
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on failure, -1 if command was backgrounded
 */
int run_command(Command *runcommand, Command *outcommand, int bg)
{
    for (int i = 0; i < flush_num_builtins(); i++)
    {
        if (strcmp((runcommand->args)[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(runcommand->args, runcommand->argc);
    }

    if (outcommand != NULL)
    {
        // Do piping
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("Pipe failed");
            return EXIT_FAILURE;
        }
        runcommand->output_pipe = pipefd[1];
        outcommand->input_pipe = pipefd[0];
    }

    int pid = fork();

    if (pid)
    {
        close(runcommand->output_pipe);
        close(runcommand->input_pipe);
        runcommand->pid = pid;
        int status;

        if (runcommand->bg)
        {
            list_push(bg_jobs, runcommand);
            printf(RESET KCYNB "[%s]" RESET " - %i\n", runcommand->cmd_str, runcommand->pid);
            return EXIT_SUCCESS;
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                return WEXITSTATUS(status);
            }
        }
    }

    // In child process
    int std_out = dup(1);

    int input_fd = 0;
    int output_fd = 1;

    if (runcommand->input_pipe > 0)
    {
        input_fd = runcommand->input_pipe;
    }
    else if (strcmp(runcommand->input_redirect, "") != 0)
    {
        if ((input_fd = open(runcommand->input_redirect, O_RDONLY)) < 0)
        {
            // Not exhaustive error handling
            printf(KYEL "Input file: " KCYNB "%s" RESET KYEL " not found\n" RESET, runcommand->input_redirect);
            exit(EXIT_FAILURE);
        }
    }
    dup2(input_fd, 0);

    if (runcommand->output_pipe > 0)
    {
        output_fd = runcommand->output_pipe;
    }
    else if (strcmp(runcommand->output_redirect, "") != 0)
    {
        if ((output_fd = open(runcommand->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) < 0)
        {
            // Not exhaustive error handling
            printf(KYEL "Output file: " KCYNB "%s" RESET KYEL " could not be created\n" RESET, runcommand->output_redirect);
            exit(EXIT_FAILURE);
        }
    }
    dup2(output_fd, 1);

    execvp((runcommand->args)[0], &(runcommand->args)[0]);

    dup2(std_out, 1);

    switch (errno)
    {
    case EACCES:
        printf(KREDB "Permission denied" RESET "\n");
        break;
    case ENOENT:
        printf(KREDB "Command not found" RESET "\n");
        break;
    default:
        printf(KREDB "Unknown error" RESET "\n");
        break;
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    char prmpt[MAX_CMD_LEN];
    char buff[MAX_CMD_LEN];
    int status;
    bg_jobs = list_init();

    generate_prompt(prmpt, sizeof(prmpt));

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    // TODO - look into prompt printing before background job
    while ((status = get_line(prmpt, buff, sizeof(buff))) != NO_INPUT)
    {
        if (status == TOO_LONG)
        {
            printf(KYEL "Command too long\n" RESET);
            continue;
        }
        else if (strlen(buff) == 0)
            continue;

        Command *commands[(strlen(buff) + 1)];
        bzero(commands, sizeof(commands));
        unsigned int commandc = 0;
        int bg = 0;

        if (parse_line(buff, commands, &commandc, &bg))
        {
            printf(KREDB "Syntax error" RESET "\n");
            continue;
        }

        for (int i = 0; i < commandc; i++)
        {
            // this is currently only status of last command, cringe
            if (i < (commandc - 1))
                status = run_command(commands[i], commands[i + 1], bg);
            else
                status = run_command(commands[i], NULL, bg);
            if (status > 0)
                break;
        }

        generate_prompt(prmpt, sizeof(prmpt));

        if (!bg)
        {
            char *status_color = (status) ? RESET KREDB : RESET KGRN;
            printf(RESET KYEL "Exit status " RESET KCYNB "[");
            for (int i = 0; i < commandc; i++)
            {
                printf("%s", commands[i]->cmd_str);
                if (i < commandc - 1)
                    printf(" | ");
                command_del(commands[i]);
            }
            printf("]%s = %i\n" RESET, status_color, status);
        }

        check_jobs();
    }

    for (Node *current = (bg_jobs->head); current != NULL; current = current->next)
    {
        Command *job = current->cmd;
        printf("%i\n", job->pid);
        kill(job->pid, SIGKILL);
        waitpid(job->pid, NULL, WNOHANG);
    }

    list_del(bg_jobs);
    printf(RESET KGRN "exit" RESET "\n");
    exit(EXIT_SUCCESS);
}