#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

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

void startup_cmd(char *cmd)
{
    char *args[MAX_ARGS];

    tokenize(args, cmd);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Failed to fork current process.\n");
        exit(1);
    }

    if (pid == 0)
    {

        char cmd_path[150];
        sprintf(cmd_path, "/bin/%s", cmd);

        // ensures we are the child process.
        if (execv(cmd_path, args) == -1) //TODO: check /bin/ and /usr/bin/
        {
            perror("Failed to execl command\n");
            exit(1);
        }
    }
}