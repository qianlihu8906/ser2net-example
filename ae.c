#include <stdio.h>
#include <errno.h>
#include <error.h>

#include <sys/epoll.h>

#define MAX_EVENTS      10

static int efd = -1;
static struct epoll_event events[MAX_EVENTS];

static void (*callback)(int fd);



int ae_init(void (*cb)(int))
{
        if(efd < 0){
                efd = epoll_create(1024);
                if(efd < 0){
                        error(-1,errno,"epoll_create failed");
                }
                callback = cb;
        }
        return 0;
}

int ae_add_event(int fd,int mask)
{
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = mask;

        if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev) == -1){
                error(-1,errno,"epoll_ctl add failed fd=%d,mask=%d\n",fd,mask);
        }
        return 0;
}

int ae_delete_event(int fd)
{
        if(epoll_ctl(efd,EPOLL_CTL_DEL,fd,NULL) == -1){
                error(-1,errno,"epoll_ctl del failed fd=%d\n",fd);
        }

        return 0;
}

int ae_loop()
{
        int n,i;
        while(1){
                n = epoll_wait(efd,events,MAX_EVENTS,-1);
                for(i=0;i<n;i++){
                        callback(events[i].data.fd);
                }
        }

}
