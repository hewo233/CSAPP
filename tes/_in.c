#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXLINE 4096  // 定义最大行长度，用于缓冲区

int main(int argc, char **argv) {
    struct addrinfo hints, *listp, *p;
    char buf[MAXLINE];  // 缓冲区，用于存储地址字符串
    int rc, flags;

    // 检查命令行参数
    if (argc != 2) {
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    // 获取addrinfo列表
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;  // 仅IPv4
    hints.ai_socktype = SOCK_STREAM;  // 仅流式套接字
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }

    // 遍历所有的返回结果
    flags = NI_NUMERICHOST;  // 获取数值形式的地址字符串
    for (p = listp; p; p = p->ai_next) {
        getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        printf("%s\n", buf);
    }

    // 清理
    freeaddrinfo(listp);
    exit(0);
}
