#include "command.h"

Command *command_init(size_t len)
{
    Command *cmd = (Command *)malloc(sizeof(Command));
    // Error handling?
    cmd->argc = 0;
    cmd->args = (char **)malloc(((len / 2) + 1) * sizeof(char *));
    cmd->output_redirects = (char **)malloc((len / 2) * sizeof(char *));
    cmd->input_redirect = "";
    cmd->pid = 0;
    cmd->outc = 0;
    cmd->bg = 0;
    return cmd;
}

int command_del(Command *cmd)
{
    free(cmd->args);
    free(cmd->output_redirects);
    free(cmd);
    // Error handling?
    return 0;
}