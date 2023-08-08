// SocketProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock.h>
#define PORT 9909

void ProcessNewMessage(int);

using namespace std;

struct sockaddr_in srv;

int nSocket;

int nMaxFd;

//socket descriptor just like file descriptor which is loaded by the the kernal object
fd_set fr, fw, fe;
int nArrClient[5];


void ProcessTheNewRequest() {

    //new connection requeuest 


    if (FD_ISSET(nSocket, &fr)) {
        int nLen = sizeof(struct sockaddr);
        int nClientSocket = accept(nSocket, NULL , &nLen);

        if (nClientSocket > 0) {
            int nIndex;
            for (nIndex = 0; nIndex < 5; nIndex++) {
                if (nArrClient[nIndex] == 0) {
                    nArrClient[nIndex] = nClientSocket;

                    send(nClientSocket,"got the connection done successfully", 37, 0);
                    break;
                }
            }

            if (nIndex == 5) cout << endl << "no space for a new connection";
        }
    }
    else {
        for (int nIndex = 0; nIndex < 5; nIndex++) {
            if ( FD_ISSET(nArrClient[nIndex], &fr)) {
                // got he new message from the client
                //just recv new message
                // just queu that for new workeds for your server to fulfull the req
                ProcessNewMessage(nArrClient[nIndex]);
            }
        }
    }

}

void ProcessNewMessage(int nClientSocket) {
    cout << endl << "Processing the new message fo client socket : " << nClientSocket;
    char buff[256 + 1] = { 0, };

    int nRet = recv(nClientSocket, buff, 256, 0);

    if (nRet < 0) {
        cout << endl << " something wrong happend .. closing the connection for client";

        // if client closes the app then it returns -1
        closesocket(nClientSocket);

        for (int nIndex = 0; nIndex < 5; nIndex++) {
            if (nArrClient[nIndex] == nClientSocket) {
                nArrClient[nIndex] = 0;
                break;

            }
        }
        
    }

    else {
        cout << endl << "the message receieved from client is : " << buff;
        send(nClientSocket, "processed your requeust", 23, 0);
        cout << endl << "**************************************";
    }

}
 
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
    nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

        for (int nIndex = 0; nIndex < 5; nIndex++) {
            if (nArrClient[nIndex] !=0) {
                FD_SET(nArrClient[nIndex], &fr);
                FD_SET(nArrClient[nIndex], &fe);
            }
        }


        nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);

        if (nRet > 0) {
            /*
            
            
            cout << "processing data ....... done processing....." << endl;
            if (FD_ISSET(nSocket, &fe)) {
                cout << endl << "there is an exception. Just get away from here";
            }
            if (FD_ISSET(nSocket, &fw)) {
                cout << endl << "ready to write something";
            }

            if (FD_ISSET(nSocket, &fr)) {
                cout << endl << "ready to read something";
                    //accept the new connection


            }
            */

            ProcessTheNewRequest();
            


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



// server is now handling one by one rather then simultaneously 
// it accumulates in the queue 
//to overcome we could you multithreading to process each client using thread
// using time slice to progress 