#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define SERV_PORT 52727 // The port the control server is listening on
const char szHost[] = "192.168.1.140";

int main(const int argc, const char* argv[]){
    // Initialise winsock
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if(WSAStartup(DllVersion, &wsaData) != 0){ // Initiate use of the winsock DLL
        ExitProcess(EXIT_FAILURE);
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(!sock){
        ExitProcess(EXIT_FAILURE);
    }

    // Get server info
    HOSTENT* host = gethostbyname(szHost);
    if(host == nullptr){
        ExitProcess(EXIT_FAILURE);
    }

    // Define server info
    SOCKADDR_IN sin;
    ZeroMemory(&sin, sizeof(sin)); // Make sure the whole struct is set to 0
    sin.sin_port = htons(SERV_PORT);
    sin.sin_family = AF_INET;
    memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr)); // Copy address into sin...S_addr

    // Connect to the server
    if(connect(sock, (const sockaddr*)&sin, sizeof(sin)) != 0){
        ExitProcess(EXIT_SUCCESS);
    }

    // Receive some data from the server
    char szBuffer[1024];
    while(1){
        recv(sock, szBuffer, 1024, 0);
        printf("%s\n", szBuffer);
        ZeroMemory(&szBuffer, sizeof(szBuffer));
    }

    closesocket(sock);
    ExitProcess(EXIT_SUCCESS); // Like return, but the OS/kernel likes it a bit more - best practice
}
