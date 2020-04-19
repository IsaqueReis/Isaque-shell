#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../include/process_handler.h"
#include "../include/error_handler.h"

int exec_and_wait(char **args, char *path)
{
    pid_t pid, wpid;
    int status = 0;

    pid = fork();
    if(pid == 0)
    {
        //executa o arquivo
        if( execvp(path, args) == -1)
            fatal_error("ishell");

        exit(EXIT_SUCCESS);
    }

    else if(pid < 0)
    {
        fatal_error("ishell");
    }

    else 
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}