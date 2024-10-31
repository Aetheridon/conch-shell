#ifndef FORKEXEC
#define FORKEXEC

#define MAX_CMD_LEN 256 // Max length of whole command, whitespaces etc.
#define MAX_ARGS 64 // Max amount of arguments within the command.
#define MAX_ARG_LEN 256 // Max length of an argument within the command.
#define REDIRECT_OUT 1 // >
#define REDIRECT_APPEND 2 // >>

void tokenize(char **args, char *cmd);
void startup_cmd(char **args);

#endif