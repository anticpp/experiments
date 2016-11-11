#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 10010
#define DEFAULT_SNDSIZE 100

void help_message(int argc, const char *argv[]) {

        printf("Usage: %s [options]\n", argv[0]);
        printf("\t--help         'Help message'\n");
        printf("\t-h host        'Send host. Default %s.'\n", DEFAULT_HOST);
        printf("\t-p port        'Send port. Default %d.'\n", DEFAULT_PORT);
        printf("\t-s size        'Send data size. Default %d.'\n", DEFAULT_SNDSIZE);
}

int main(int argc, const char *argv[])
{
    int help = 0;
    char host[1024] = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    int sndsize = DEFAULT_SNDSIZE;

    int err = 0;
    for (int i=1; i<argc; i++ ) {

        if( argv[i][0]!='-' ) {
            printf("Error argument '%s'\n", argv[i]);
            err = 1;
            break;
        }

        if( strcmp(argv[i], "--help")==0 ) {
            help = 1;
        } else if ( strcmp(argv[i], "-h")==0 ) {
            if( i+1>=argc ) {
                err = 1;
                break;
            }
            i ++;
            strncpy(host, argv[i], sizeof(host)-1);
        } else if ( strcmp(argv[i], "-p")==0 ) {
            if( i+1>=argc ) {
                err = 1;
                break;
            }
            i ++;
            port = atoi( argv[i] );
        } else if( strcmp(argv[i], "-s")==0 ) {
            if( i+1>=argc ) {
                err = 1;
                break;
            }
            i ++;
            sndsize = atoi( argv[i] );
        } else {
            printf("Error argument '%s'\n", argv[i]);
            err = 1;
            break;
        }

    }

    if (err==1) {
        printf("Invalid arguments.\n");
        help_message(argc, argv);
        return 2;
    }

    if (help==1) {
        help_message(argc, argv);
        return 0;
    }

    printf("Host: %s, Port: %d, Send size: %d\n", host, port, sndsize);

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if( sfd<0 ) {
        printf("Create socket error\n");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);

    char *sndbuf = (char *)malloc(sndsize);
    memset(sndbuf, 0x00, sndsize);
    ssize_t ss = sendto(sfd, sndbuf, sndsize, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ss<0) {
        printf("sendto error\n");
        return 1;
    }
    
    printf("%d bytes sent\n", ss);

    return 0;
}
