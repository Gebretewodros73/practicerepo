#ifdef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 32
#define MAX_CMD_LEN 1024

typedef struct {
	char *command;
	size_t len;
	char *stkn;
	pid_t my_pid;
	int status;
	char **arr;
} Shell;

void printsenv(void);
void process_input(Shell *shell);
char *find_command(char *cmd);

#endif
