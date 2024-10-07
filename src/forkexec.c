#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "forkexec.h"

void startup_cmd(char cmd[])
{
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
        // example syntax: if (execl("/bin/ls", "ls", "-l", NULL) == -1) 
        if (execl(cmd_path, cmd, NULL) == -1) //TODO: check /bin/ and /usr/bin/, implement arguments
        {
            perror("Failed to execl command\n");
            exit(1);
        }
    }
}