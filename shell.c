#include "shell.h"

#define MAX_CMD_LEN 1024
#define MAX_INPUT_LEN 4096

// Custom getline function
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);

int main(int argc, char **argv)
{
    char input[MAX_INPUT_LEN];
    char *stkn, *command;
    size_t len = 0;

    while (1)
    {
        printf("$ ");
        fflush(stdout);

        // Read input using custom getline function
        ssize_t bytes_read = my_getline(&command, &len, stdin);

        // Handle errors and end of input
        if (bytes_read == -1)
        {
            fprintf(stderr, "Error reading input\n");
            continue;
        }
        else if (bytes_read == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        // Remove trailing newline from input
        if (command[bytes_read - 1] == '\n')
        {
            command[bytes_read - 1] = '\0';
        }

        // Tokenize input
        stkn = strtok(command, " \n");
        char **arr = malloc(sizeof(char *) * (MAX_ARGS + 1));

        arr[0] = stkn;
        int i = 1;

        while (stkn != NULL)
        {
            stkn = strtok(NULL, " \n");
            arr[i] = stkn;
            i++;
        }
        arr[i] = NULL;

        // Execute command
        if (arr[0] == NULL)
        {
            free(arr);
            continue;
        }
        if (strcmp(arr[0], "exit") == 0 && (arr[1] == NULL))
        {
            printf("Exiting shell...\n");
            break;
        }
        else if (strcmp(arr[0], "env") == 0 && (arr[1] == NULL))
        {
            print_environment();
        }
        else
        {
            execute_run(arr);
        }
        free(arr);
    }

    free(command);
    return 0;
}

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
    static char buf[MAX_INPUT_LEN];
    static size_t buf_len = 0;
    size_t line_len = 0;

    // Handle buffer overflow
    if (buf_len >= MAX_INPUT_LEN)
    {
        fprintf(stderr, "Input too long\n");
        return -1;
    }

    // Read input into buffer
    ssize_t bytes_read = read(STDIN_FILENO, buf + buf_len, MAX_INPUT_LEN - buf_len);
    if (bytes_read == -1)
    {
        perror("read");
        return -1;
    }

    // Find newline character in buffer
    char *newline = memchr(buf + buf_len, '\n', bytes_read);
    if (newline != NULL)
    {
        line_len = newline - buf + 1;
        buf[line_len - 1] = '\0';
    }

    // Update lineptr and n
    if (line_len > 0)
    {
        *lineptr = buf;
        *n = line_len;
    }

    // Update buffer length and contents
    buf_len += bytes_read;
    if (line_len > 0)
    {
        buf_len -= line_len;
        memmove(buf, buf + line_len, buf_len);
    }

    return line_len;
}
