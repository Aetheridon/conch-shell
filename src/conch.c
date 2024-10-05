#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int sh_loop(void);

int main(int argc, char *argv[])
{
    sh_loop();
    return 0;
}

int sh_loop(void)
{
    while (1)
    {
        char cmd[100];

        printf("\n$ ");

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

        printf("%s", cmd);
    }

    return 0;

}
