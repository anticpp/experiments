#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 10010
#define DEFAULT_SNDSIZE 100
#define DEFAULT_SOCKET_SNDBUF 0
#define DEFAULT_COUNT 1

void help_message(int argc, const char *argv[]) {

        printf("Usage: %s [options]\n", argv[0]);
        printf("\t--help         'Help message'\n");
        printf("\t-h host        'Send host. Default %s.'\n", DEFAULT_HOST);
        printf("\t-p port        'Send port. Default %d.'\n", DEFAULT_PORT);
        printf("\t-s size        'Send data size. Default %d.'\n", DEFAULT_SNDSIZE);
        printf("\t-ssb size      'Set socket SNDBUF size. Default %d. Don't set if 0.'\n", DEFAULT_SOCKET_SNDBUF);
        printf("\t-c count       'Send count. Default %d.'\n", DEFAULT_COUNT);
}

int main(int argc, const char *argv[])
{
    int help = 0;
    char host[1024] = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    int sndsize = DEFAULT_SNDSIZE;
    int sndbuf_size = DEFAULT_SOCKET_SNDBUF; 
    int count = DEFAULT_COUNT;

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
        } else if( strcmp(argv[i], "-ssb")==0 ) {
            if( i+1>=argc ) {
                err = 1;
                break;
            }
            i ++;
            sndbuf_size = atoi( argv[i] );
        } else if( strcmp(argv[i], "-c")==0 ) {
            if( i+1>=argc ) {
                err = 1;
                break;
            }
            i ++;
            count = atoi( argv[i] );
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

    printf("Host: %s, Port: %d, Send size: %d, Send count %d\n", host, port, sndsize, count);

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if( sfd<0 ) {
        printf("Create socket error\n");
        return 1;
    }

    // Set SNDBUF
    if (sndbuf_size>0){
        if ( setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, sizeof(sndbuf_size))<0 ) {
            printf("setsockopt error\n");
        } else {
            printf("Set SNDBUF %d\n", sndbuf_size);
        }
    }

    // Get SNDBUF
    {
        int opt=0;
        int optlen=sizeof(optlen);
        if ( getsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &opt, &optlen)<0 ) {
            printf("getsockopt error\n");
        } else {
            printf("Socket SNDBUF: %d\n", opt);
        }
    }

    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);

    char *sndbuf = (char *)malloc(sndsize);
    memset(sndbuf, 0x00, sndsize);
    for(int i=0; i<count; i++){
        ssize_t ss = sendto(sfd, sndbuf, sndsize, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (ss<0) {
            printf("sendto error, %s\n", strerror(errno));
            return 1;
        }
        printf("[%d] %d bytes sent\n", i, ss);
    }
    

    return 0;
}
