/// Implementation for the socket-server-test program.
///@file socket-server-test.cpp

// File Header.
#include "socket-server.h"

// System Headers.
#include <iostream>
#include <thread>
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
// Non blocking server.
#include <fcntl.h>

// Application Headers.







/// Run a blocking socket server.
void blockingSocketServer()
{
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
        inet_ntop(AF_INET, &clientAddress.sin_addr, clientName, sizeof(clientName));
        std::cout << "Connection from " << clientName << std::endl;

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
}

/// Run a simple non blocking socket server.
void nonBlockingSocketServer()
{
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
        inet_ntop(AF_INET, &clientAddress.sin_addr, clientName, sizeof(clientName));
        std::cout << "Connection from " << clientName << std::endl;

        // Wait for a second.
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Receiving data.
        char buffer[1024] = { 0 };
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                std::cout << "No data available now." << std::endl;
            }
            else
            {
                std::cout << "recv error." << std::endl;
            }
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
        }
        else
        {
            std::cout << "Receive: " << buffer << std::endl;

            // Replying data.
            const char* message = "Thank-you";
            std::cout << "Reply: " << message << std::endl;
            send(clientSocket, message, strlen(message), 0);
        }

        count++;
    }

    // Closing the socket.
    close(serverSocket);
}


/// Run a non blocking socket server.
/// This does not seem to work.
void nonBlockingSocketServerNotWorking()
{
    // Create a socket.
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cout << "Socket error." << std::endl;
        return;
    }

    // Specify the address.
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    // serverAddress.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Connect to socket.
    if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cout << "Socket connect error." << std::endl;
        close(serverSocket);
        return;
    }

    // Set socket to non-blocking mode.
    int flags = fcntl(serverSocket, F_GETFL, 0);
    if (flags == -1)
    {
        std::cout << "fntl get error." << std::endl;
        close(serverSocket);
        return;
    }
    if (fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        std::cout << "fntl set error." << std::endl;
        close(serverSocket);
        return;
    }

    int count = 0;
    while (count < 200)
    {
        // Accepting the connection request.
        //sockaddr_in clientAddress;
        //socklen_t clientLength = sizeof(clientAddress);
        //char clientName[128];
        //int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
        //inet_ntop(AF_INET, &clientAddress.sin_addr, clientName, sizeof(clientName));
        //std::cout << "Connection from " << clientName << std::endl;

        // Wait for a second.
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Receiving data.
        char buffer[1024] = { 0 };
        ssize_t bytesReceived = recv(serverSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                std::cout << "No data available now." << std::endl;
            }
            else
            {
                std::cout << "recv error." << std::endl;
            }
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
        }
        else
        {
            std::cout << "Receive: " << buffer << std::endl;

            // Replying data.
            const char* message = "Thank-you";
            std::cout << "Reply: " << message << std::endl;
            send(serverSocket, message, strlen(message), 0);
        }

        count++;
    }

    // Closing the socket.
    close(serverSocket);

}


/// Main entry point for the socket-server-test program.
int main
(
    int             argc,   // The number of command line arguments.
    const char**    argv    // The command line arguments.
)
{
    // Welcome messge.
    std::cout << "Hello from socket-server-test." << std::endl;

    // Run one of the socket servers.
    if (true)
    {
        blockingSocketServer();
    }
    else
    {
        nonBlockingSocketServer();
    }

    // Farewell message.
    std::cout << "Goodbye from socket-server-test." << std::endl;

    // return success.
    return 0;

}
