// C++ socket client that connects to a server

#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 

#define SERV_PORT 52727
#define SERV_IP "192.168.1.140"

/*
Not going to worry about adding error checking, doing that would make the code easier to reverse engineer
*/

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    struct sockaddr_in serv_addr; // Create a struct to hold the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); // Set the port to connect to
    serv_addr.sin_addr.s_addr = inet_addr(SERV_IP); // Convert the IP string into a format that can be used by the socket
    connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); // Connect to the server
    // Once connected to the server, listen for data
    // For now, just print the data to the screen and echo it back to the server
    while(1){
        char buffer[1024] = {0}; // Create a buffer to hold the data, ensure it is empty
        int bytes_read = read(fd, buffer, 1024);
        if(bytes_read > 0){
            std::cout << buffer << std::endl;
            write(fd, buffer, bytes_read);
        }
    }
    return 0;
}