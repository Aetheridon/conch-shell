#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>

#include "forkexec.h"
#include "builtin.h"
#include "terminal.h"

int sh_loop(void);
void sigint_handler(int sig_num);

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);
    
    sh_loop();
    return 0;
}

int sh_loop(void)
{
    enable_raw_mode();

    while (1)
    {
        char cmd[MAX_CMD_LEN];
        char *args[MAX_ARGS];

        refresh_prompt();

        int i = 0;
        char ch;

        while ((ch = read_key()) != '\n')
        {
            if (ch == 12)
            {    
                clear_screen();
                refresh_prompt();
                i = 0;
                continue;
            }

            if (ch == 127)
            { // handles backspace
                if (i > 0) {
                    i--;
                    printf("\b \b");
                    fflush(stdout);
                }
                continue;
            }

            if (i < MAX_CMD_LEN - 1) // builds characters in the command into an array.
            {
                cmd[i++] = ch;
                putchar(ch);
                fflush(stdout);
            }
        }

        printf("\n");
        cmd[i] = '\0'; 

        tokenize(args, cmd);

        if (args[0] == NULL)
        {
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            break;
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

    disable_raw_mode();
    return 0;
}

void sigint_handler(int sig_num) //TODO: move to separate file.
{
    // Cleanup for sigint.
    printf("\nExiting...\n");
    disable_raw_mode();
    exit(0);
}
