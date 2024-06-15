#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#define MAXLINE 8192

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void SIGPIPE_handler(int sig)
{
    fprintf(stderr, "SIGPIPE caught\n");
    return;
}

typedef struct URI
{
    char hostname[MAXLINE];
    char port[MAXLINE];
    char path[MAXLINE];
}tURI;

void build_header(char *header, tURI *pURI, rio_t *client_rio)
{
    strcpy(header, "GET ");
    strcat(header, pURI->path);
    strcat(header, " HTTP/1.0\r\n");

    strcat(header, "Host: ");
    strcat(header, pURI->hostname);
    if(strcmp(pURI->port, "80"))
    {
        strcat(header, ":");
        strcat(header, pURI->port);
    }
    strcat(header, "\r\n");

    strcat(header, user_agent_hdr);

    char buf[MAXLINE];
    while(Rio_readlineb(client_rio, buf, MAXLINE) > 0)
    {
        if(strcmp(buf, "\r\n") == 0)
            break;
        if(strstr(buf, "Connection") && strstr(buf, "Proxy-Connection") )
        {
            strcat(header, buf);
        }
    }

    strcat(header, "\r\n");
}

void doit(int connfd)
{
    rio_t rio, server_rio;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    
    Rio_readinitb(&rio, connfd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);

    if(strcasecmp(method, "GET"))
    {
        fprintf(stderr, "GET is not allowed\n");
        return;
    }

    tURI *pURI = (tURI *)Malloc(sizeof(tURI));
    parse_uri(uri, pURI);

    char server_request[MAXLINE];
    build_header(server_request, pURI, &rio);

    int serverfd = Open_clientfd(pURI->hostname, pURI->port);
    if(serverfd < 0)
    {
        fprintf(stderr, "Open_clientfd error\n");
        return;
    }

    
}

int main(int argc, char **argv) 
{
    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int listenfd, connfd;
    socklen_t clientlen;
    char hostname[MAXLINE], port[MAXLINE];
    struct sockaddr_storage clientaddr;

    signal(SIGPIPE, SIGPIPE_handler);

    listenfd = Open_listenfd(argv[1]);
    if(listenfd < 0)
    {
        fprintf(stderr, "Open_listenfd error\n");
        exit(1);
    }
    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);

        doit(connfd);

        Close(connfd);
    }

    return 0;
}