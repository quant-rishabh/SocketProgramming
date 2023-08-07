// SocketProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock.h>
#define PORT 9909

using namespace std;

struct sockaddr_in srv;

int nMaxFd;

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
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket >= 0) std::cout << "it is working";



    //initlize the enviroment for sockaddr sturcture
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT); //HOST TO NETWORK SOCKS NETWORK BYTE ORDER REQURIED AT THE TIME OF NETWORK CALL
    srv.sin_addr.s_addr = INADDR_ANY; // assigning address to s addr
    memset(&(srv.sin_zero), 0, 8);

    // set sock opp
    int nOptVal = 0;
    int nOptLen = sizeof(nOptVal);
    // so that multiple application can use the same  socket
    nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);

    if (!nRet ) {
        
        cout << endl << "The setsock opt call successful";
    }
    else {
        cout << endl << "The setsock opt call failed";
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    // bind the socket to local port
    



    nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));

    if (nRet < 0) {
        cout << endl << nRet << "  fail to bind the local port";
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "The socket opened successfully" << " " << nSocket;
    }

    //blocking vs non blocking 
//you wait until it returns (io blocking) multithreaded is needed
//(non blocking) always running doesnt wait for any return
    //every socket by default is blocking socket

    /*

    u_long optval = 0;

    nRet = ioctlsocket(nSocket, FIONBIO, &optval);

    if (nRet != 0) {
        cout << endl << "ioctlsocket call failed";
    }
    else
    {
        cout << endl << "ioctlsocket call passed";
    }

*/

    //listen the request from cloent(queues the requeuests)
    //backlog , how many requeust can client can make to server

    nRet = listen(nSocket, 5);

    if (nRet < 0) {
        cout << endl << "Fail to start listen to slocal port";
        WSACleanup();
        exit(EXIT_FAILURE);

    }
    else {
        cout << endl << "started listening to local port";
    }

    // keep waiting for new req
    
    while (1) {
        //The select() call monitors activity on a set of sockets looking for sockets ready for reading, writing, or with an exception condition pending.
        nMaxFd = nSocket;

        timeval tv;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        FD_ZERO(&fr);
        FD_ZERO(&fw);
        FD_ZERO(&fe);

        FD_SET(nSocket, &fr);
        FD_SET(nSocket, &fe);



        nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);

        if (nRet > 0) {

        }
        else if (nRet == 0) {
            cout << "nothing on the port :" << PORT << endl;
        }
        else {
            cout << "failed" << endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        Sleep(2000);
  }
}

