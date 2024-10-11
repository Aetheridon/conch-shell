#ifndef FORKEXEC
#define FORKEXEC

#define MAX_CMD_LEN 256 // Max length of whole command, whitespaces etc.
#define MAX_ARGS 64 // Max amount of arguments within the command.
#define MAX_ARG_LEN 256 // Max length of an argument within the command.

void startup_cmd(char *cmd);

#endif