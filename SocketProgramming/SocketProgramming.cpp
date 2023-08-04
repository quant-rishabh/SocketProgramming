// SocketProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock.h>
#define PORT 9909

using namespace std;

struct sockaddr_in srv;

int main()
{   // Initialize the wsa variables

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
    srv.sin_family = htons(PORT); //HOST TO NETWORK SOCKS NETWORK BYTE ORDER REQURIED AT THE TIME OF NETWORK CALL
    srv.sin_addr.s_addr = INADDR_ANY; // assigning address to s addr
    memset(&(srv.sin_zero), 0, 8);

    // bind the socket to local port
     
}
