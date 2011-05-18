all:server client client1
stub:mig.defs
	mig -prefix S_ mig.defs
server:server.c migServer.c
	gcc server.c migServer.c -o server -lmachuser -lports -Wall -D_GNU_SOURCE  -I. -ltrivfs -lfshelp -lshouldbeinlibc -lthreads
client:client.c migUser.c
	gcc client.c migUser.c -o client  -Wall -D_GNU_SOURCE -I.
client1: client1.c migUser.c
	gcc client1.c migUser.c -o client1 -Wall -D_GNU_SOURCE -I.
clean:
	rm -f *.o server client
start: server client
	settrans -ac /tmp/trans server
	ps -x | grep server
end:
	settrans -fp /tmp/trans
