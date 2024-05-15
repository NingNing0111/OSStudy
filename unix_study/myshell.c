#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// input command str, get commands
// example:
// input: "ls -l -h -a"
// return: ["ls","-l","-h","-a",NULL]
char **get_input(char *input)
{
    char **command = malloc(8 * sizeof(char *));
    if (command == NULL)
    {
        perror("malloc failed.");
        exit(-1);
    }
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL)
    {
        command[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}

int cd_command(char *path)
{
    return chdir(path); // chdir will change the current path.
}

int main()
{

    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;

    signal(SIGINT, SIG_IGN);
    while (1)
    {
        input = readline("MyShell> ");
        if (input == NULL)
        { /* Exit on Ctrl-D */
            printf("\n");
            exit(0);
        }
        command = get_input(input);
        if (strcmp(command[0], "cd") == 0)
        {
            if (cd_command(command[1]) < 0)
            {
                perror(command[1]);
            }
            continue;
        }

        child_pid = fork();
        if (child_pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            if (execvp(command[0], command) < 0)
            {
                perror(command[0]);
                exit(-1);
            }
        }
        else if (child_pid < 0)
        {
            printf("Failed to create new process.");
            exit(-1);
        }
        else
        {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }
        free(input);
        free(command);
    }

    return 0;
}