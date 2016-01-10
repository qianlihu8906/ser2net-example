#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void set_address(char *hname,char *sname,struct sockaddr_in *sap,char *protocal)
{
        struct servent *sp;
        struct hostent *hp;
        char *endptr;
        short port;

        bzero(sap,sizeof(*sap));
        sap->sin_family = AF_INET;
        if(hname != NULL){
                if(!inet_aton(hname,&sap->sin_addr)){
                        hp = gethostbyname(hname);
                        if(hp == NULL)
                                error(-1,0,"unknown host:%s\n",hname);
                        sap->sin_addr = *(struct in_addr *)hp->h_addr;
                }
        }else
                sap->sin_addr.s_addr = htonl(INADDR_ANY);
        port = strtol(sname,&endptr,0);
        if( *endptr == '\0')
                sap->sin_port = htons(port);
        else{
                sp = getservbyname(sname,protocal);
                if(sp == NULL){
                        error(1,0,"unknown service:%s\n",sname);
                }
                sap->sin_port = sp->s_port;
        }
}

int tcp_server(char *hname,char *sname)
{
        struct sockaddr_in local;
        int sd;
        const int on = 1;

        set_address(hname,sname,&local,"tcp");
        sd = socket(AF_INET,SOCK_STREAM,0);
        if(sd < 0){
                error(-1,errno,"socket call failed");
        }
        if( setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on)) )
                error(-1,errno,"setsockopt failed");

        if(bind(sd,(struct sockaddr *)&local,sizeof(local)))
                error(-1,errno,"bind failed");

        if(listen(sd,SOMAXCONN))
                error(-1,errno,"listen failed");

        return sd;
}


static void set_nonblocking(int fd) 
{
        int flag = fcntl(fd, F_GETFL, 0); 
        if ( -1 == flag ) { 
                return;
        }

        fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int tcp_wait_for_connect(int sd)
{
        struct sockaddr_in peer;
        socklen_t peerlen = sizeof(peer);

        int sd1 = accept(sd,(struct sockaddr *)&peer,&peerlen);
        if(sd1 < 0)
                error(-1,errno,"accept failed");
        set_nonblocking(sd1);
        return sd1;
}



#if 0
int main()
{
        int server_fd = tcp_server("0.0.0.0","61000");
        int client_fd = tcp_wait_for_connect(server_fd);

        printf("client_fd = %d\n",client_fd);
}
#endif
