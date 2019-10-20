//-------------------------------Server.cpp-------------------------------------
//Description:
//------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/types.h>    // socket, bind
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include <arpa/inet.h>    // inet_ntoa
#include <netdb.h>        // gethostbyname
#include <unistd.h>       // read, write, close
#include <strings.h>      // bzero
#include <netinet/tcp.h>  // SO_REUSEADDR
#include <sys/uio.h>      // writev

//----------------------------------main----------------------------------------
//Description: The primary driver for the server. It creates the initial socket
//             via the server port number being passed in as an argument,
//             then listens for a connecting host and establishes threads for
//             each connection.
//------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
  
  return 0;
}
