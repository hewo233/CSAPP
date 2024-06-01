#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int signum) {
    printf("Received SIGINT\n");
    sleep(5); // 模拟处理信号需要的时间
}

int main() {
    signal(SIGINT, handler);
    while(1) {
        printf("Sleeping for 1 second\n");
        sleep(1);
    }
    return 0;
}