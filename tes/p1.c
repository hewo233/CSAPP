#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void handler(int signum) {
    printf("Got a SIGINT\n");
    sigset_t currentset;
    sigprocmask(SIG_BLOCK, NULL, &currentset);
    int res = sigismember(&currentset, SIGINT);
    printf("SIGINT is blocked ? : %d \n", res);
}

int main() {
    signal(SIGINT, handler);
    sleep(10);
    printf("Sleeping for 10 seconds\n");
    return 0;
}