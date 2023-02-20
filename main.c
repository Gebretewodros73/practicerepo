#include "shell.h"

int main(int argc, char **argv)
{
    Shell shell = {NULL, 0, NULL, 0, 0, NULL};

    while (1)
    {
        printf("$ ");

        getline(&shell.command, &shell.len, stdin);

        shell.stkn = strtok(shell.command, " \n");

        shell.arr = malloc(sizeof(char *) * MAX_ARGS);
        shell.arr[0] = shell.stkn;

        int i = 1;
        while (shell.stkn != NULL && i < MAX_ARGS)
        {
            shell.stkn = strtok(NULL, " \n");
            shell.arr[i] = shell.stkn;
            i++;
        }

        if (strcmp(shell.arr[0], "exit") == 0 && (shell.arr[1] == NULL))
        {
            free(shell.arr);
            free(shell.command);
            exit(0);
        }

        if (strcmp(shell.arr[0], "env") == 0 && (shell.arr[1] == NULL))
        {
            printsenv();
            free(shell.arr);
            continue;
        }

        process_input(&shell);
        free(shell.arr);
    }

    free(shell.command);
    return (0);
}
