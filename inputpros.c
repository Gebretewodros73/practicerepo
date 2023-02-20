#include "shell.h"

char *find_command(char *cmd)
{
    char *path = getenv("PATH");
    char *tok = strtok(path, ":");
    char *cmd_path = malloc(strlen(cmd) + 2 + strlen(tok));
    sprintf(cmd_path, "%s/%s", tok, cmd);

    while (tok != NULL)
    {
        if (access(cmd_path, X_OK) == 0)
        {
            return cmd_path;
        }
        else
        {
            tok = strtok(NULL, ":");
            if (tok != NULL)
            {
                sprintf(cmd_path, "%s/%s", tok, cmd);
            }
        }
    }

    free(cmd_path);
    return NULL;
}
