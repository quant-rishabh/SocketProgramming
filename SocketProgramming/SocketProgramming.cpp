// SocketProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock.h>
#define PORT 9909

using namespace std;

struct sockaddr_in srv;

int main()
{   // Initialize the wsa variables

    int nRet = 0;

    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws)< 0) {
        std::cout << "wsa data not working";
    }
    else {
        std::cout << "wsa initilized by windows" << endl;
    }

    // Initialize the socket
    int nSocket = socket(AF_INET , SOCK_STREAM,IPPROTO_TCP);
    if (nSocket >= 0) std::cout << "it is working";



    //initlize the enviroment for sockaddr sturcture
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT); //HOST TO NETWORK SOCKS NETWORK BYTE ORDER REQURIED AT THE TIME OF NETWORK CALL
    srv.sin_addr.s_addr = INADDR_ANY; // assigning address to s addr
    memset(&(srv.sin_zero), 0, 8);

    // bind the socket to local port

    nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));

    if (nRet < 0) {
        cout << endl << nRet << "  fail to bind the local port";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "The socket opened successfully" << nSocket;
    }

    //listen the request from cloent(queues the requeuests)
    //backlog , how many requeust can client can make to server

    nRet = listen(nSocket, 5);

    if (nRet < 0) {
        cout << endl << "Fail to start listen to slocal port";
        exit(EXIT_FAILURE);

    }
    else {
        cout << endl << "started listening to local port";
    }

    // keep waiting for new req

}
