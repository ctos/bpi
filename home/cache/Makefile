CC = gcc
MIG = mig
CFLAGS = -Wall -g -D_GNU_SOURCE
LDFLAGS = -lthreads -lports -ltrivfs -lshouldbeinlibc -lfshelp -lmachuser
INCLUDES = -I.
MIGFLAGS = -prefix Cache_

all:server user

stub:cache.defs
	$(MIG) $(MIGFLAGS) $^
server: server.c cacheServer.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o $@
user: user.c cacheUser.c
	$(CC) $^ $(CFLAGS) $(INCLUDES) -o $@
clean: 
	rm -rf *.o server user
start: server user
	settrans -ac /tmp/cache server
	ps -x | grep server
end:
	settrans -fs /tmp/cache

