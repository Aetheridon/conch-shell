// handling terminal configs such as entering / exiting raw mode
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "terminal.h"
#include "forkexec.h"

struct termios orig_termios;

void enable_raw_mode(void)
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        perror("Failed to get terminal attributes");
        exit(1);
    }

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char read_key(void)
{
    char ch;

    if (read(STDIN_FILENO, &ch, 1) == -1)
    {
        perror("Error reading input");
        exit(1);
    }

    return ch;
}

void clear_screen(void)
{
    char *args[] = {"clear", NULL};
    startup_cmd(args);
}

void refresh_prompt(void)
{
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

    fflush(stdout);
}

void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}