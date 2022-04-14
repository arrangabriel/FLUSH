#include "utils.h"

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

int refresh_jobs(List *jobs)
{
    Node *current = jobs->head;
    while (current != NULL)
    {
        Command *job = current->cmd;
        int status;
        if (waitpid(job->pid, &status, WNOHANG) == 0)
        {
            current = current->next;
            continue;
        }

        // Each finished job gets its own line, should maye be combined for a piped command
        char *status_color = (status) ? RESET KREDB : RESET KGRN;
        printf(RESET KYEL "Exit status " RESET KCYNB "[");
        printf("%s", job->cmd_str);
        printf("]%s = %i\n" RESET, status_color, status);

        current = current->next;
        list_remove(jobs, job);
        command_del(job);
    }
    return EXIT_SUCCESS;
}

int get_io(Command *cmd, int *input_fd, int *output_fd)
{
    if (cmd->input_pipe > 0)
    {
        *input_fd = cmd->input_pipe;
    }
    else if (strcmp(cmd->input_redirect, "") != 0)
    {
        if ((*input_fd = open(cmd->input_redirect, O_RDONLY)) < 0)
        {
            printf(KYEL "Input file: " RESET KCYNB "%s" RESET KYEL " not found\n", cmd->input_redirect);
            return EXIT_FAILURE;
        }
    }

    if (cmd->output_pipe > 0)
    {
        *output_fd = cmd->output_pipe;
    }
    else if (strcmp(cmd->output_redirect, "") != 0)
    {
        if ((*output_fd = open(cmd->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) < 0)
        {
            // Not exhaustive error handling
            printf(KYEL "Output file: " KCYNB "%s" RESET KYEL " could not be created\n" RESET, cmd->output_redirect);
            return (EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}