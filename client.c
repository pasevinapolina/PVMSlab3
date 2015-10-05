#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct sockaddr_in server;
	int socketfd;
	int bytes_count;
    char buf[256];

    if (argc != 3) {
        puts("Usage: client <server ip> <command>");
        return 1;
    }

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        puts("Error: Failed to create socket!\n");
        return 2;
    }
	puts("Socket created\n");


    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(8042);

    if (connect(socketfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        puts("Error: Failed to connect to server!\n");
        close(socketfd);
        return 3;
    }
	puts("Connected to server\n");

    if (send(socketfd, argv[2], strlen(argv[2]), 0) == -1) {
        puts("Error: Failed to send the command!\n");
        close(socketfd);
        return 4;   
    }

    
    while ((bytes_count = recv(socketfd, buf, sizeof(buf)/sizeof(buf[0]), 0)) > 0) {
        fwrite(buf, bytes_count, 1, stdout);
    }

    close(socketfd);

    return 0;
}
