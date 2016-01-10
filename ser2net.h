#ifndef SER2NET_H__
#define SER2NET_H__

#include <sys/epoll.h>
struct ser2net_config{
        const char *ttypath;
        const char *uart_speed;
        const char *ipaddr;
        const char *port;
};
struct ser2net_config* get_config_with_args(int argc,char **argv);
struct ser2net_config* get_config();

int open_seriport(const char *ttypath,const char* uart_speed);

int tcp_server(const char *ipaddr,const char* port);
int tcp_wait_for_connect(int fd);

#define READ_EVENT      EPOLLIN
int ae_init(void (*cb)(int));
int ae_add_event(int fd,int mask);
int ae_delete_event(int fd);
int ae_loop();

#endif
