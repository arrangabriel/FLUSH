#include "command.h"

Command *command_init(size_t len)
{
    Command *cmd = (Command *)malloc(sizeof(Command));
    cmd->argc = 0;
    cmd->args = (char **)malloc(((len / 2) + 1) * sizeof(char *));
    cmd->cmd_str = (char *)malloc(sizeof(char*));
    cmd->output_redirect = "";
    cmd->input_redirect = "";
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