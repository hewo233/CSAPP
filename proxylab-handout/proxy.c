#include <stdio.h>
#include <stdlib.h>
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
    strcat(header, " HTTP/1.1\r\n");

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

    printf("DEBUGGGGGGGGGG:!! %s\n", header);
}

void parse_uri(char *uri, tURI *pURI)
{
    char *ptr = strstr(uri, "http://");
    if(ptr == NULL)
    {
        fprintf(stderr, "Invalid URI\n");
        return;
    }
    ptr += strlen("http://");

    char *ptr2 = strstr(ptr, ":");
    if(ptr2 == NULL)
    {
        strcpy(pURI->port, "80");
    }
    else
    {
        sscanf(ptr2, ":%s", pURI->port);
    }

    ptr2 = strstr(ptr2, "/");
    if(ptr2 == NULL)
    {
        fprintf(stderr, "Invalid URI\n");
        return;
    }
    
    sscanf(ptr2, "%s", pURI->path);
    
    ptr2 = strstr(ptr, ":");
    if(ptr2 == NULL)
    {
        sscanf(ptr, "%[^/]", pURI->hostname);
    }
    else

        doit(connfd);

        Close(connfd);
    }

    return 0;
}