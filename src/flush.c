#include "parsing.h"
#include "command.h"
#include "list.h"
#include "utils.h"
#include <signal.h>
#include <errno.h>

#define MAX_CMD_LEN 1024
#define MAX_PRMPT_LEN PATH_MAX + (2 * sizeof(char))

extern int errno;

List *bg_jobs;

void sig_handler(int signo) {
    // Not sure if this should do something, other than catch the signal
}

int flush_cd(char **args, unsigned int argc);
int flush_exit(char **args, unsigned int argc);
int flush_jobs(char **args, unsigned int argc);

char *builtin_str[] = {
    "cd",
    "exit",
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
    // The fact that this needs the bg_jobs makes the whole ordeal annoyingly coupled
    // List *jobs = (List *)args[0];
    // Do this to remedy showing finished jobs
    refresh_jobs(bg_jobs);

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

/**
 * @brief Runs a command structure
 *
 * @param runcommand The command to run
 * @param outcommand The command to run if the command is a pipe
 * @return Status
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
        int pipefd[2];
        if (pipe(pipefd) == -1)
            return EXIT_FAILURE;

        runcommand->output_pipe = pipefd[1];
        outcommand->input_pipe = pipefd[0];
    }

    int pid = fork();

    if (pid)
    {
        close(runcommand->output_pipe);
        runcommand->pid = pid;
        int status;

        if (runcommand->bg)
        {
            list_push(bg_jobs, runcommand);
            printf(RESET KCYNB "[%s]" RESET " - %i\n", runcommand->cmd_str, runcommand->pid);
        }
        else if (outcommand == NULL)
        {
            // Checking for next command creates a pseudo background job of current command
            // waitpid errors when the process is not running, but I don't think it matters
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                return WEXITSTATUS(status);
        }
        // We do not save state of intermittent commands, but this allows main loop to continue executing
        // Another option is to check state of each command after the final one has finished executing.
        return EXIT_SUCCESS;
    }

    // In child process
    int std_out = dup(1);

    int input_fd = 0;
    int output_fd = 1;

    if (get_io(runcommand, &input_fd, &output_fd))
        exit(EXIT_FAILURE);

    dup2(input_fd, 0);
    dup2(output_fd, 1);

    execvp((runcommand->args)[0], &(runcommand->args)[0]);

    // If exec failed
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
    char prmpt[MAX_PRMPT_LEN];
    char buff[MAX_CMD_LEN];
    int status;
    bg_jobs = list_init();

    generate_prompt(prmpt, sizeof(prmpt));

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    // Main loop
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
        memset(commands, 0, sizeof(commands));
        unsigned int commandc = 0;
        int bg = 0;

        if (parse_line(buff, commands, &commandc, &bg))
        {
            printf(KREDB "Syntax error" RESET "\n");
            continue;
        }

        for (int i = 0; i < commandc; i++)
        {
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
            print_commands(commands, commandc, status);
            for (int i = 0; i < commandc; i++)
                command_del(commands[i]);
        }

        refresh_jobs(bg_jobs);
    }

    for (Node *current = (bg_jobs->head); current != NULL; current = current->next)
    {
        Command *job = current->cmd;
        kill(job->pid, SIGKILL);
        waitpid(job->pid, NULL, WNOHANG);
    }

    list_del(bg_jobs);
    printf(RESET KGRN "exit" RESET "\n");
    exit(EXIT_SUCCESS);
}