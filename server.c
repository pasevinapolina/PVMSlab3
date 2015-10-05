#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

void * process_request(void * arg) {
    long socket = (long) arg;
    char buf[1024] = { 0 };
    int bytes_count = recv(socket, buf, sizeof(buf)/sizeof(buf[0]), 0);
    FILE *pipe = popen(buf, "r");

    while (bytes_count = fread(buf, 1, sizeof(buf)/sizeof(buf[0]), pipe)) {
        write(socket, buf, bytes_count);
    }
    pclose(pipe);

    close(socket);
}

int main(int argc, char **argv)
{
    int socket_listener;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t sock_len;
	int client_socket;
	int bytes_count;
    char buf[1024];

	socket_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_listener == -1) {
        puts("Error: Failed to create socket!\n");
        return 2;
    }
	puts("Socket created\n");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8042);

    if (bind(socket_listener, (struct sockaddr *) &server, sizeof(server)) == -1) {
        puts("Error: Failed to bind the socket!");
        close(socket_listener);
        return 3;    
    }

    listen(socket_listener, 5);
	puts("Waiting for client connections\n");

    
    sock_len = sizeof(struct sockaddr_in);
    FILE *pipe;
    pthread_t thread;
    while (1) {
        memset(buf, 0, sizeof(buf)/sizeof(buf[0]));
		client_socket = accept(socket_listener, (struct sockaddr *) &client, &sock_len);
        if (client_socket == -1) {
            continue;
        }

#if PROC        
        pid_t pid = fork();
        if (pid == -1) {
            puts("Error: Failed to create process!");
            return 4;
        }
        if (!pid) {
            process_request((void *) client_socket);
            _exit(0);
        } else {
            close(client_socket);
        }
#else
        if (pthread_create(&thread, NULL, process_request, (void *)(long) client_socket)) {
            puts("Error: Failed to create thread!");
            return 4;
        }
#endif
    }

    close(socket_listener);

    return 0;
}
