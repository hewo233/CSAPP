#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h> // Include the necessary header file

int main()
{
    pid_t pid;

    int x = 1;
    pid = fork();
    printf("pid is : %d",pid);
    if(pid == 0)
    {
        printf("Child: x = %d\n", ++x);
        exit(0);
    }
    printf("Parent: x = %d\n", --x);
    exit(0);
}