#include "../../apue.h"
#include <strings.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>

#define BUFLEN 1024
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int, struct sockaddr*, socklen_t, int);

void serve(int sockfd)
{
    int clfd, n;
    FILE* fp;
    char buf[BUFLEN];

    for ( ; ; )
    {
        clfd = accept(sockfd, NULL, NULL);
        if (clfd < 0)
        {
            syslog(LOG_ERR, "alistturingmachine: accept error: %s", strerror(errno));
            exit(1);
        }
        
        if ((n = read(clfd, buf, BUFLEN)) > 0)
        {
            if ((fp = popen(buf, "r")) == NULL)
            {
                sprintf(buf, "error: %s\n", strerror(errno));
                send(clfd, buf, strlen(buf), 0);
            }
            else
            {
                while (fgets(buf, BUFLEN, fp) != NULL)
                    send(clfd, buf, strlen(buf), 0);
                pclose(fp);
            }
        }
        bzero(buf, strlen(buf));
        close(clfd);
    }
}

int main(int argc, char* argv[])
{
    // struct addrinfo* ailist;
    // struct addrinfo* aip;
    // struct addrinfo hint;
    int sockfd;  // , err, n;
    // char* host;

    if (argc != 1)
        err_quit("usage: alistturingmachine");
// #ifdef _SC_HOST_NAME_MAX
//     n = sysconf(_SC_HOST_NAME_MAX);
//     if (n < 0)
// #endif        
//         n = HOST_NAME_MAX;
//     host = malloc(n);
//     if (host == NULL)
//         err_sys("malloc error");
//     if (gethostname(host, n) < 0)
//         err_sys("gethostname error");
    // daemonize("ruptimed1");
    // hint.ai_flags = AI_CANONNAME;
    // hint.ai_family = 0;
    // hint.ai_socktype = SOCK_STREAM;
    // hint.ai_protocol = 0;
    // hint.ai_addrlen = 0;
    // hint.ai_canonname = NULL;
    // hint.ai_addr = NULL;
    // hint.ai_next = NULL;
    // if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    // {
    //     syslog(LOG_ERR, "ruptimed1: getaddrinfo error: %s", gai_strerror(err));
    //     exit(1);
    // }
    // for (aip = ailist; aip != NULL; aip = aip->ai_next)
    // {

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6121);
    if ((sockfd = initserver(SOCK_STREAM, (struct sockaddr*) &servaddr,
                             sizeof(servaddr), QLEN)) >= 0)
    {
        serve(sockfd);
        exit(0);
    }

    // }
    exit(1);
}
