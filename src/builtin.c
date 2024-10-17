#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int cd(char **args)
{
    if (args[1] == NULL)
    {
        // No argument provided, change to HOME directory
        if (chdir(getenv("HOME")) != 0)
        {
            perror("cd");
        }
    }

    else
    {
        // Argument provided, change to specified directory
        if (chdir(args[1]) != 0)
        {
            perror("cd");
        }
    }
    
    return 1;
}