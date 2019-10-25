//-------------------------------Retriever.cpp----------------------------------
//Description: This program is the client/retriever application for a
//             client/server HTTP request homework assignment for CSS432 at the
//             University of Washington. It's a Linux-based client/retriever
//             application that
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <sstream>
#include <sys/types.h>    // socket, bind
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include <arpa/inet.h>    // inet_ntoa
#include <netdb.h>        // gethostbyname
#include <unistd.h>       // read, write, close
#include <strings.h>      // bzero
#include <netinet/tcp.h>  // SO_REUSEADDR
#include <sys/uio.h>      // writev

#define BUF_SIZE 1024

using namespace std;

//--------------------------------saveHTML--------------------------------------
//Description: Receives the buffer at the head of the HTML file. Reads remainder
//             of file, prints to the console and saves it to a file in the root
//             directory of this program.
//------------------------------------------------------------------------------
void saveHTML(int &clientSd, char &response)
{
  cout << "-------------------------------------------------------------------" << endl;
  ofstream page;
  page.open("peng.html");
  if (!page.is_open())
  {
    cout << "Error occurred. Unable to open the file." << endl;
  }
  cout << response; // Adding back in the bracket
  page << response; // Adding back in the bracket
  while (read(clientSd, &response, 1) > 0)
  {
    cout << response;
    page << response;
  }
  cout << "\n-------------------------------------------------------------------" << endl;
  cout << "Writing peng.html..." << endl;
  page.close();
  cout << "-------------------------------------------------------------------" << endl;
} // end of saveHTML

//--------------------------------parseHeader-----------------------------------
//Description: Receives the socket descriptor and parses the header to
//             determine the status of the HTTP request. If it's a 200 request,
//             it calls saveHTML to print and save the file. Otherwise, it prints
//             the status code and response of the error.
//------------------------------------------------------------------------------
void parseHeader(int &clientSd)
{
  char response;
  string contents;
  while (true)
  {
    read(clientSd, &response, 1);
    contents.push_back(response);
    if (response == '\n')
    {
      break;
    }
  }
  if (contents.find("200") != string::npos)
  {
    while (read(clientSd, &response, 1) > 0)
    {
      if (response == '<')
      {
        saveHTML(clientSd, response);
      }
    }
  }
  else
  {
    cout << "-------------------------------------------------------------------" << endl;
    cout << contents;
    cout << "-------------------------------------------------------------------" << endl;
  }
} // end of parseHeader

//----------------------------------main----------------------------------------
//Description: The primary driver for the retriever. It creates the initial socket
//             via the server port number being passed in as an argument, as
//             well as the hostname of the server, and the server path for the
//             HTML file being requested. Note: exclude the http:// or https://
//             prefix to the hostname, and omit the leading slash for the path
//             name, EX: faculty.washington.edu /yangpeng
//------------------------------------------------------------------------------
int main (int argc, char* argv[])
{
  // Checking to ensure the correct number of arguments are being received
  if (argc < 5)
  {
    perror("Incorrect number of arguments entered. Please run again.");
    exit(1);
  }

  // Assign hostname and path from arguments
  char* serverHostname  = argv[1];
  char* serverPath      = argv[2];
  int port              = atoi(argv[3]);
  char* requestType     = argv[4];

  // Check if hostname includes http:// or https:// prefixes and remove if so
  char *temp;
  if ((temp = strstr(serverHostname, "http://")) != NULL)
  {
    serverHostname = serverHostname + 7; // Pushes pointer for array beyond
    cout << serverHostname << endl;      // undesired chars
  }
  if  ((temp = strstr(serverHostname, "https://")) != NULL)
  {
    serverHostname = serverHostname + 8;
  }

  // Convert hostname to IP and ensure hostname IP could be resolved
  struct hostent* host = gethostbyname(serverHostname);
  if (host == NULL)
  {
    perror("Unable to resolve hostname IP.");
    exit(1);
  }

  // Create Socket Function
  int clientSd;             // Declare a client socket descriptor
  sockaddr_in sendSockAddr; // Declare a sockaddr_in

  // Zero initialize it
  bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
  // Set its data members
  sendSockAddr.sin_family       = AF_INET; //Address Family internet
  sendSockAddr.sin_addr.s_addr  = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
  sendSockAddr.sin_port         = htons(port);

  // Open a stream-oriented socket with the internet address Family - check for failure
  if ((clientSd = socket(AF_INET, SOCK_STREAM, 0)) <= -1)
  {
    perror("Unable to open a client socket.");
    exit(1);
  }

  // End Create Socket Function
  cout << "Socket successfully created..." << endl;

  // Connect to server socket - check for failed connection
  int connectStatus = connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
  if (connectStatus < 0)
  {
    perror("\nFailed to connect to the server.");
    exit(1);
  }
  cout << "\nConnection to server successful..." << endl;

  // Create array of buffer size for GET request
  char getRequestBuffer[BUF_SIZE];

  string httpType(requestType);
  // Build an HTTP GET request
  stringstream ss;
  ss << httpType << " " << serverPath << " HTTP/1.1\r\n"
     << "Host: " << serverHostname  << "\r\n"
     << "\r\n";
  string request = ss.str();

  cout << "\nSending HTTP " << httpType << " Request..." << endl;
  write(clientSd, request.c_str(), request.length());

  parseHeader(clientSd);

  cout << "\nClosing connection...";
  close(clientSd);

  return 0;
} // end of main
