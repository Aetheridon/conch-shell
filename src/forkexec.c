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

        if (strcmp(token, "|") == 0)
        {
            args[arg_count++] = "|";
            token = strtok(NULL, " ");
            continue;
        }

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

void split_commands(char **args, char **cmd1, char **cmd2)
{
    int i = 0, j = 0;
    
    while (args[i] != NULL && strcmp(args[i], "|") != 0)
    {
        cmd1[i] = args[i];
        i++;
    }

    cmd1[i] = NULL;
    
    if (args[i] != NULL)
    {
        i++;
        while (args[i] != NULL)
        {
            cmd2[j] = args[i];
            i++;
            j++;
        }
    }

    cmd2[j] = NULL;
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
    char *cmd1[MAX_ARGS];
    char *cmd2[MAX_ARGS];

    char *outfile;
    int redirect_type;
    
    int has_pipe = 0;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            has_pipe = 1;
            break;
        }
    }

    if (has_pipe) {

        int pipefd[2];

        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return;
        }

        split_commands(args, cmd1, cmd2);
        
        pid_t pid1 = fork();

        if (pid1 < 0)
        {
            perror("fork");
            return;
        }
        
        if (pid1 == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            
            if (execvp(cmd1[0], cmd1) == -1)
            {
                perror("execvp");
                exit(1);
            }
        }

        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            perror("fork");
            return;
        }
        
        if (pid2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            
            parse_redirection(cmd2, &outfile, &redirect_type);
            
            if (redirect_type != 0)
            {
                int flags = (redirect_type == REDIRECT_OUT) ? 
                    O_WRONLY | O_CREAT | O_TRUNC : 
                    O_WRONLY | O_CREAT | O_APPEND;
                
                int fd = open(outfile, flags, 0644);

                if (fd < 0)
                {
                    perror("open");
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            
            if (execvp(cmd2[0], cmd2) == -1)
            {
                perror("execvp");
                exit(1);
            }
        }

        close(pipefd[0]);
        close(pipefd[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    }

    else 
    {
        parse_redirection(args, &outfile, &redirect_type);
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return;
        }

        if (pid == 0)
        {
            if (redirect_type != 0)
            {
                int flags = (redirect_type == REDIRECT_OUT) ?
                    O_WRONLY | O_CREAT | O_TRUNC :
                    O_WRONLY | O_CREAT | O_APPEND;
                
                int fd = open(outfile, flags, 0644);
                
                if (fd < 0)
                {
                    perror("open");
                    exit(1);
                }
                
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            
            if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(1);
            }
        }

        else
        {
            wait(NULL);
        }
    }
}