#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "forkexec.h"

void tokenize(char **args, char *cmd)
{
    int arg_count = 0;    

    char *token = strtok(cmd, " ");

    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " "); // proceeds to point to the next token.
    }

    args[arg_count] = NULL;
}

void startup_cmd(char **args)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Failed to fork current process.\n");
        exit(1);
    }

    if (pid == 0) // fork returns 0 if we're the child process
    {
        if (execvp(args[0], args) == -1)
        {
            perror("Failed to execute command\n");
            exit(1);
        }
    }

    else
    {
        wait(NULL);
    }
}