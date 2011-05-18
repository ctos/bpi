CC = gcc
MIG = mig
CFLAGS = -Wall -g -D_GNU_SOURCE
MIGFLAGS = -prefix Cookie_
LDFLAGS = -lthreads -lports -ltrivfs -lshouldbeinlibc -lfshelp -lmachuser
INCLUDES = -I.

all:server user
stub:cookie.defs
	$(MIG) $(MIGFLAGS) $^
user:user.c cookieUser.c
	$(CC) $^ $(CFLAGS) $(INCLUDES) -o $@
server:server.c cookieServer.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $^ -o $@
clean:
	rm -rf *.o server user
start:server user
	settrans -ac /tmp/cookie server
	ps -x | grep server
end:
	settrans -fg /tmp/cookie





	
