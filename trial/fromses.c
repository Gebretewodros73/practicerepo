#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

void printsenv(void);
void process_input(char **arr);

int main(int argc, char **argv)
{
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

        char **arr = malloc(sizeof(char *) * 32);
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
            printsenv();
            free(arr);
            continue;
        }

        process_input(arr);
        free(arr);
    }

    free(command);
    return (0);
}

void printsenv(void)
{
    int i;
    i = 0;
    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i]);
        i++;
    }
}

void process_input(char **arr)
{
    pid_t my_pid;
    int status;

    my_pid = fork();
    if (my_pid == -1)
    {
        perror("Error");
        exit(1);
    }
    else if(my_pid == 0)
    {
        if (execve(arr[0], arr, environ) == -1)
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
