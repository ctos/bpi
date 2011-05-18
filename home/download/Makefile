CC = gcc
MIG = mig
MIGFLAG = -prefix Download_
LDFLAGS = -lthreads -lports -ltrivfs -lshouldbeinlibc -lfshelp -lmachuser
INCLUDES = -I.
CFLAGS = -Wall -g -D_GNU_SOURCE

all:server user
test: test.c
	$(CC) $^  $(CFLAGS) -o $@
stub:download.defs
	$(MIG) $(MIGFLAG) $^
server:server.c downloadServer.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o $@
user: user.c downloadUser.c
	$(CC) $^ $(CFLAGS) $(INCLUDES) -o $@
clean:
	rm -rf *.o server user
start: server user
	settrans -ac /tmp/download server
	ps -x| grep server
end:
	settrans -fg /tmp/download
