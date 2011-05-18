all: server client client1

server: server.c
	gcc server.c -o server -lmachuser -lports -ltrivfs -D_GNU_SOURCE
	
client:client.c
	gcc client.c -o client -D_GNU_SOURCE
client1:client1.c
	gcc client1.c -o client1 -D_GNU_SOURCE
start:server client
	settrans -ac /tmp/trans1 server
	ps -x | grep server
clean:
	-rm test
