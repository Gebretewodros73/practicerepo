#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 32
#define MAX_ENV_VARS 256

struct shell {
    char **environ;
};

void printsenv(struct shell *sh);
void process_input(struct shell *sh, char **arr);
char *find_command(char *cmd, struct shell *sh);

int main(int argc, char **argv)
{
    struct shell sh;
    sh.environ = environ;

    char *command = NULL;
    size_t len = 0;
    char *stkn;
    pid_t my_pid;
    int status, i;

    while (1)
    {
        printf("$ ");
        getline(&command, &len, stdin);

        stkn = strtok(command, " \n");

        char **arr = malloc(sizeof(char *) * MAX_ARGS);
        arr[0] = stkn;

        i = 1;
        while (stkn != NULL)
        {
            stkn = strtok(NULL, " \n");
            arr[i] =stkn;
            i++;
        }

        if (strcmp(arr[0], "exit") == 0 && (arr[1] == NULL))
        {
            free(arr);
            free(command);
            exit(0);
        }

        if (strcmp(arr[0], "env") == 0 && (arr[1] == NULL))
        {
            printsenv(&sh);
            free(arr);
            continue;
        }

        process_input(&sh, arr);
        free(arr);
    }

    free(command);
    return (0);
}

void printsenv(struct shell *sh)
{
    int i;
    i = 0;
    while (sh->environ[i] != NULL)
    {
        printf("%s\n", sh->environ[i]);
        i++;
    }
}

void process_input(struct shell *sh, char **arr)
{
    pid_t my_pid;
    int status;

    char *cmd = find_command(arr[0], sh);

    if (cmd == NULL)
    {
        printf("%s: Command not found\n", arr[0]);
        return;
    }

    my_pid = fork();
    if (my_pid == -1)
    {
        perror("Error");
        exit(1);
    }
    else if(my_pid == 0)
    {
        if (execve(cmd, arr, sh->environ) == -1)
        {
            perror(arr[0]);
            exit(1);
        }
    }
    else
    {
        wait(&status);
    }
}

char *find_command(char *cmd, struct shell *sh)
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

