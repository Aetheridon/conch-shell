#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "forkexec.h"
#include "builtin.h"

int sh_loop(void);
void sigint_handler(int sig_num);

int main(int argc, char *argv[])
{
    sh_loop();
    return 0;
}

int sh_loop(void)
{
    signal(SIGINT, sigint_handler);

    while (1)
    {
        char cmd[MAX_CMD_LEN];
        char *args[MAX_ARGS];
        char cwd[1024];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s$ ", cwd);
        }

        else
        {
            perror("getcwd() error");
            printf("$ ");
        }

        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
        {
            perror("fgets failed to read from stdin.\n");
            return 1;
        }

        cmd[strcspn(cmd, "\n")] = 0; 

        tokenize(args, cmd);

        if (args[0] == NULL) continue;

        if (strcmp(args[0], "exit") == 0)
        {
            return 0;
        }

        else if (strcmp(args[0], "cd") == 0)
        {
            cd(args);   
        }

        else
        {
            startup_cmd(args);
        }

    }

    return 0;
}

void sigint_handler(int sig_num) //TODO: move to separate file.
{
    // Cleanup for sigint.
    printf("\nExiting...\n");
    exit(0);
}