#include "command.h"
#include "utils.h"

Command *command_init(size_t len)
{
    Command *cmd = (Command *)malloc(sizeof(Command));
    cmd->argc = 0;
    cmd->args = (char **)malloc(((len / 2) + 1) * sizeof(char *));
    cmd->cmd_str = (char *)malloc(sizeof(char*));
    cmd->output_redirect = "";
    cmd->input_redirect = "";
    cmd->input_pipe = -1; 
    cmd->output_pipe = -1;
    cmd->pid = 0;
    cmd->bg = 0;
    return cmd;
}

int command_del(Command *cmd)
{
    free(cmd->args);
    free(cmd->cmd_str);
    free(cmd);
    // Error handling?
    return 0;
}

void print_commands(Command *commands[], int commandc, int status)
{
    char *status_color = (status) ? RESET KREDB : RESET KGRN;
    printf(RESET KYEL "Exit status " RESET KCYNB "[");
    for (int i = 0; i < commandc; i++)
    {
        printf("%s", commands[i]->cmd_str);
        if (i < commandc - 1)
            printf(" | ");
    }
    printf("]%s = %i\n" RESET, status_color, status);
}