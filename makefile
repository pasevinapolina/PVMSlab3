PROC=0

all:
	gcc server.c -pthread -Dproc=$(PROC) -o server
	gcc client.c -o client
clean:
	rm -f client
	rm -f server
