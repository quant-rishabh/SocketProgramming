// SocketProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock.h>
#define PORT 9909

using namespace std;

struct sockaddr_in srv;

//socket descriptor just like file descriptor which is loaded by the the kernal object
fd_set fr, fw, fe;

int main()
{   // Initialize the wsa variables

    int nRet = 0;

    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        std::cout << "wsa data not working";
    }
    else {
        std::cout << "wsa initilized by windows" << endl;
    }

    // Initialize the socket
    int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nClientSocket >= 0) std::cout << "it is working";



    //initlize the enviroment for sockaddr sturcture
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT); //HOST TO NETWORK SOCKS NETWORK BYTE ORDER REQURIED AT THE TIME OF NETWORK CALL
    srv.sin_addr.s_addr = inet_addr("127.0.0.1"); // assigning address to s addr
    memset(&(srv.sin_zero), 0, 8);

    nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (nRet < 0) {
        cout << endl << nRet << "  connect failed";
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "Connected to the server " << " " << nClientSocket;

        char buff[255] = { 0, };
        recv(nClientSocket, buff, 255, 0);
        cout << endl << buff;

        cout << endl << "Now send your messages to server : ";


        while (1) {
            fgets(buff, 256, stdin);
            send(nClientSocket, buff, 256, 0);
            recv(nClientSocket, buff, 256, 0);

            cout << endl << buff << endl << "now send next message : ";


        }



    }
}

