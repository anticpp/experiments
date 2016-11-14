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

#define DEFAULT_HOST "*"
#define DEFAULT_PORT 10010
#define DEFAULT_RCVBUF 0

void help_message(int argc, const char *argv[]) {

    printf("Usage: %s [options]\n", argv[0]);
    printf("\t--help         'Help message'\n");
    printf("\t-h host        'Listen ip. Default %s.'\n", DEFAULT_HOST);
    printf("\t-p port        'Listen port. Default %d.'\n", DEFAULT_PORT);
    printf("\t-rb RCVBUF     'Set RCVBUF size. Default %d.'\n", DEFAULT_RCVBUF);
}

int main(int argc, const char *argv[])
{
    int help = 0;
    char host[1024] = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    int rcvbuf_size = DEFAULT_RCVBUF;

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
        } else if ( strcmp(argv[i], "-rb")==0 ) {
            if( i+1>=argc ) {
                err = 1;
                break;
            }
            i ++;
            rcvbuf_size = atoi( argv[i] );
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

    printf("Host: %s, Port: %d\n", host, port);

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if( sfd<0 ) {
        printf("Create socket error\n");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if( strcmp(host, "*")==0) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        addr.sin_addr.s_addr = inet_addr(host);
    }

    if( bind(sfd, (struct sockaddr *)&addr, sizeof(addr))<0 ) {
        printf("Bind error\n");
        return 1;
    }
    printf("Bind success\n");

    // Set RCVBUF
    if (rcvbuf_size>0){
        if ( setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size))<0 ) {
            printf("setsockopt error\n");
        } else {
            printf("Set RCVBUF %d\n", rcvbuf_size);
        }
    }

    // Get RCVBUF
    {
        int opt=0;
        int optlen;
        if ( getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &opt, &optlen)<0 ) {
            printf("getsockopt error\n");
        } else {
            printf("Check RCVBUF: %d\n", opt);
        }
    }
    
    ssize_t ss;
    char rcvbuf[100*1024];
    struct sockaddr_in caddr;
    socklen_t len;
    while(1) {
        
        ss = recvfrom(sfd, rcvbuf, sizeof(rcvbuf), 0, (struct sockaddr *)&caddr, &len);
        if (ss<0) {
            printf("Recv error\n");
            break;
        }
        printf("Recv %d bytes\n", ss);

    }
    return 0;
}
