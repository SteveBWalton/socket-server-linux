/// Implementation for the socket-server-test program.
///@file socket-server-test.cpp

// File Header.
#include "socket-server.h"

// System Headers.
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

// Application Headers.



/// Main entry point for the socket-server-test program.
int main
(
    int             argc,   // The number of command line arguments.
    const char**    argv    // The command line arguments.
)
{
    // Welcome messge.
    std::cout << "Hello from socket-server-test." << std::endl;

    // Create a socket.
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify the address.
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Binding socket.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cout << "Error on binding." << std::endl;
    }

    // Listening to the assigned socket.
    std::cout << "Listening on port 8080" << std::endl;
    listen(serverSocket, 5);

    int count = 0;
    while (count < 2)
    {
        // Accepting the connection request.
        sockaddr_in clientAddress;
        socklen_t clientLength = sizeof(clientAddress);
        char clientName[128];
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
        std::cout << "Connection from " << inet_ntop(AF_INET, &clientAddress.sin_addr, clientName, sizeof(clientName)) << std::endl;

        // Receiving data.
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Receive: " << buffer << std::endl;

        // Replying data.
        const char* message = "Thank-you";
        std::cout << "Reply: " << message << std::endl;
        send(clientSocket, message, strlen(message), 0);

        count++;
    }

    // Closing the socket.
    close(serverSocket);

    // Farewell message.
    std::cout << "Goodbye from socket-server-test." << std::endl;

    // return success.
    return 0;
}

