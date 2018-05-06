#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifndef PORT
  #define PORT 30000
#endif
#define BUF_SIZE 128

int main(void) {
    // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("client: socket");
        exit(1);
    }

    // Set the IP and port of the server to connect to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 1) {
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }

    // Connect to the server.
    if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        exit(1);
    }

    printf("Username: ");
    char username[256];
    scanf("%s",username);

    int write_user = write(sock_fd, username, strlen(username));
    //error?

    // Read input from the user, send it to the server, and then accept the
    // echo that returns. Exit when stdin is closed.
    char buf[BUF_SIZE + 1];
    int max_fd = sock_fd;
    fd_set server_socket, socket_cpy;
    FD_ZERO(&server_socket);
    FD_SET(max_fd, &server_socket);
    FD_SET(STDIN_FILENO, &server_socket);

    while (1) {

        socket_cpy = server_socket;

        if( select(max_fd+1, &socket_cpy, NULL, NULL, NULL) == -1)
        {
            perror("select");
            close(sock_fd);
            return -1;
        }

        if(FD_ISSET(STDIN_FILENO, &socket_cpy))
        {
            int bytes_read = read(STDIN_FILENO, buf, BUF_SIZE);
            buf[bytes_read] = '\0'; //Because Andrew's paranoia spread like wildfire
            write(sock_fd, buf, bytes_read);
        }

        if(FD_ISSET(sock_fd, &socket_cpy))
        {
            int bytes_read = read(sock_fd, buf, BUF_SIZE);
            buf[bytes_read] = '\0';
            write(STDOUT_FILENO, buf, bytes_read);
        }

    }

    close(sock_fd);
    return 0;
}
