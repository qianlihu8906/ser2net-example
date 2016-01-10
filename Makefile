#CROSS=arm-linux-gnueabihf-

CC=$(CROSS)gcc
CPP=$(CROSS)g++
LD=$(CROSS)ld
AS=$(CROSS)as
AR=$(CROSS)ar
OBJCOPY=$(CROSS)objcopy
OBJDUMP=$(CROSS)objdump

CFLAGS =  -Wall -I.  -D_GNU_SOURCE 
LDFLAGS = -lm 

COPY        := cp
MKDIR       := mkdir -p
MV          := mv
RM          := rm -f
DIRNAME     := dirname

EXEC = ser2net


SRCS := ae.c seriport.c socket.c config.c ser2net.c

OBJS := $(SRCS:%.c=%.o)

$(EXEC):$(OBJS)
	$(CC)  $^ -o $@ $(LDFLAGS)

$(OBJS):$(SRCS)
	$(CC)  $(CFLAGS) -c $^
clean:
	$(RM) *.o $(EXEC)
