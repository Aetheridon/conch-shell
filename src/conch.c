#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "forkexec.h"

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
        char cmd[100];

        wait(0);

        printf("$ ");

        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
        {
            perror("fgets failed to read from stdin.\n");
            return 1;
        }

        cmd[strcspn(cmd, "\n")] = 0; 

        if (strcmp(cmd, "exit") == 0)
        {
            return 0;
        }

        startup_cmd(cmd);

    }

    return 0;

}

void sigint_handler(int sig_num) //TODO: move to separate file.
{
    // Cleanup for sigint.
    printf("\nExiting...\n");
    exit(0);
}
