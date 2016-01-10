#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>

static int look_up_uart_speed(const char *speed)
{
        char *endptr;
        int uart_speed = strtol(speed,&endptr,0);
        if( *endptr != '\0' )
                error(-1,0,"uart_speed strtol failed");
	switch(uart_speed){
	case 9600:		return B9600;
	case 19200: 	        return B19200;
	case 38400:		return B38400;
	case 57600:		return B57600;
	case 115200:	        return B115200;
	default:		return -1;
	}
}

int open_seriport(const char *ttypath,const char* uart_speed)
{
        int fd = open(ttypath,O_RDWR|O_NOCTTY|O_NONBLOCK);
        if(fd < 0){
                error(-1,errno,"open %s error",ttypath);
        }
	struct termios tios;
	memset(&tios,0,sizeof(struct termios));
	/* 将串口设置为原始模式 */
	cfmakeraw(&tios);
	/* baud rate */
	cfsetospeed(&tios,look_up_uart_speed(uart_speed));
	cfsetispeed(&tios,look_up_uart_speed(uart_speed));
	/* 流控制选项 */
	tios.c_iflag &= ~IXOFF;
	tios.c_cflag &= ~CRTSCTS;

        tios.c_cflag |= CS8;

        //控制模式 parity check
        tios.c_cflag &= ~PARENB;     //no parity check

        tios.c_cflag &= ~CSTOPB; //1 stop bits
	/* 设置串口缓冲队列 */
	tios.c_cc[VTIME] = 0;
	tios.c_cc[VMIN]  = 0;
	/* 使串口设置生效 */
	if(tcsetattr(fd,TCSADRAIN,&tios) < 0){
                error(-1,errno,"tcsetattr error");
	}

        return fd;
}
