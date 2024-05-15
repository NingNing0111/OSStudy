#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t child_pid;
    pid_t wait_result;

    int stat_loc;

    // fork child process
    child_pid = fork();

    // The child process
    if (child_pid == 0)
    {
        printf("### Child ###\nCurrent PID: %d and Child PID: %d\n",
               getpid(), child_pid);
        sleep(1);
    }
    else if (child_pid < 0)
    {
        printf("Failed to create new process.");
        exit(-1);
    }
    else
    {
        // Wait for the child process to complete execution.
        wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        printf("### Parent ###\nCurrent PID: %d and Child PID: %d\n",
               getpid(), child_pid);
    }

    return 0;
}