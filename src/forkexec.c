#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "forkexec.h"

void tokenize(char **args, char *cmd) {
    int arg_count = 0;
    char *token = strtok(cmd, " ");
    
    while (token != NULL && arg_count < MAX_ARGS - 1)
    {
        if (strchr(token, '>') != NULL)
        {
            char *pos = strstr(token, ">>");

            if (pos == token)
            {
                args[arg_count++] = ">>";
                token = strtok(NULL, " ");
                continue;
            }
        
            pos = strchr(token, '>');

            if (pos == token)
            {
                args[arg_count++] = ">";
                token = strtok(NULL, " ");
                continue;
            }
        }
        
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    
    args[arg_count] = NULL;
}


void parse_redirection(char **args, char **outfile, int *redirect_type)
{
    *redirect_type = 0;
    *outfile = NULL;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            *redirect_type = REDIRECT_OUT;
            *outfile = args[i + 1];
            args[i] = NULL;
            break;
        }

        else if (strcmp(args[i], ">>") == 0)
        {
            *redirect_type = REDIRECT_APPEND;
            *outfile = args[i + 1];
            args[i] = NULL;
            break;
        }
    }
}

void startup_cmd(char **args)
{
    char *outfile;
    int redirect_type;

    parse_redirection(args, &outfile, &redirect_type);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Failed to fork current process.\n");
        exit(1);
    }

    if (pid == 0) { // Child process

        if (redirect_type != 0)
        {
            int flags, fd;
                
            if (redirect_type == REDIRECT_OUT)
            {
                flags = O_WRONLY | O_CREAT | O_TRUNC;
            }

            else
            {
                flags = O_WRONLY | O_CREAT | O_APPEND;
            }
                
            fd = open(outfile, flags, 0644);

            if (fd < 0)
            {
                perror("Failed to open output file");
                exit(1);
            }
                
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("Failed to redirect stdout");
                exit(1);
            }

            close(fd);
        }
        
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
