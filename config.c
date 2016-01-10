#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "ser2net.h"

static struct ser2net_config config = {
        .ttypath = "/dev/ttySAC3",
        .uart_speed = "115200",
        .ipaddr = "0.0.0.0",
        .port = "61000",
};

struct ser2net_config* get_config_with_args(int argc,char **argv)
{
        int opt;
        while( (opt=getopt(argc,argv,"c:s:p:h:")) != -1 ){
                switch(opt){
                        case 'c':
                                config.ttypath = optarg;
                                break;
                        case 's':
                                config.uart_speed = optarg;
                                break;
                        case 'p':
                                config.port = optarg;
                                break;
                        case 'h':
                                config.ipaddr = optarg;
                                break;
                        default:
                                fprintf(stderr,"Usage:%s [-csph] \n"
                                               "\t -c ttypath \n "
                                               "\t -s uart_speed\n"
                                               "\t -p port\n"
                                               "\t -h host\n",argv[0]);
                                exit(-1);
                }
        }
        return &config;
}

struct ser2net_config* get_config()
{
        return &config;
}


#if 0
int main(int argc,char **argv)
{
        struct ser2net_config *c = get_config_with_args(argc,argv);
        fprintf(stdout,"ttypath=%s,uart_speed=%s,ipaddr=%s,port=%s\n",c->ttypath,c->uart_speed,c->ipaddr,c->port);
        return 0;
}
#endif
