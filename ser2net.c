#include <stdio.h>
#include <errno.h>
#include <error.h>

#include <unistd.h>
#include <signal.h>
#include "ser2net.h"

static int server_fd = -1;
static int client_fd = -1;
static int seriport_fd = -1;

void do_with_fd(int fd)
{
        char buf[1024] = {0};
        int r = read(fd,buf,sizeof(buf));
        if(r <= 0){
                if(fd == seriport_fd){
                        if(fd < 0)
                                error(-1,errno,"read seriport error fd=%d\n",fd);
                }else if(fd == client_fd){
                        ae_delete_event(fd);
                        close(fd);
                        client_fd = tcp_wait_for_connect(server_fd);
                        ae_add_event(client_fd,READ_EVENT);
                }
                return;
        }
        if(fd == seriport_fd){
                write(client_fd,buf,r);
        }else{
                write(seriport_fd,buf,r);
        }

}

int main(int argc,char **argv)
{
        struct ser2net_config *c = get_config_with_args(argc,argv);

        signal(SIGHUP,SIG_IGN);
        signal(SIGPIPE,SIG_IGN);

        seriport_fd = open_seriport(c->ttypath,c->uart_speed);
        server_fd = tcp_server(c->ipaddr,c->port);
        client_fd = tcp_wait_for_connect(server_fd);

        ae_init(do_with_fd);
        ae_add_event(client_fd,READ_EVENT);
        ae_add_event(seriport_fd,READ_EVENT);

        ae_loop();

        return 0;
}
