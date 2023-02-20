#include "shell.h"

void process_input(Shell *shell)
{
    char *cmd = find_command(shell->arr[0]);

    if (cmd == NULL)
    {
        printf("%s: Command not found\n", shell->arr[0]);
        return;
    }

    shell->my_pid = fork();
    if (shell->my_pid == -1)
    {
        perror("Error");
        exit(1);
    }
    else if (shell->my_pid == 0)
    {
        if (execve(cmd, shell->arr, NULL) == -1)
        {
            perror(shell->arr[0]);
            exit(1);
        }
    }
    else
    {
        wait(&shell->status);
    }
}
