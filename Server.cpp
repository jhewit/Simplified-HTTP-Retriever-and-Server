//-------------------------------Server.cpp-------------------------------------
//Description:
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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
#include <time.h>

#define BUF_SIZE 1024
#define PORT 5859

using namespace std;

// Structure for holding data for an individual thread.
struct thread_data
{
  int sd;
};

//-------------------------------parsePath--------------------------------------
//Description:
//Assumption: Per assignment specifications, Server only handles GET requests
//            and assumes all requests are GET.
//------------------------------------------------------------------------------
void *parsePath(void *data)
{
  /*
  char buffer[BUF_SIZE];
  char request;
  string path;
  bool found = false;
  while (true)
  {
    read(((thread_data *)data)->sd, &request, 1);
    if (found == true)
    {
      if (request == ' ')
      {
        break;
      }
      path.push_back(request);
    }
    if (request == ' ')
    {
      found = true;
    }
  }
  */
//******************************************************************************
// 100% WORKING!!
//******************************************************************************
  // FILE * fileptr;
  // time_t timenow;
  // struct tm * timeinfo;
  // time (&timenow);
  // timeinfo = localtime(&timenow);
  // char * header, * request, * path, * newpath, * host; //keep
  // char get[3], http[9];                               //keep
  // char filepath[BUF_SIZE];
  // char http_not_found[] = "HTTP/1.1 404 Not Found\n";
  // char http_ok[] = "HTTP/1.1 200 OK\n";
  // char buffer[BUF_SIZE];                              //keep
  // char contentType[] = "text/html";
  // char connection[] = "close";
  // header = (char*)malloc(BUF_SIZE*sizeof(char));    //keep
  // request = (char*)malloc(BUF_SIZE*sizeof(char));   //keep
  // path = (char*)malloc(BUF_SIZE*sizeof(char));      //keep
  // newpath = (char*)malloc(BUF_SIZE*sizeof(char));   //keep
  // recv(((thread_data *)data)->sd, request, 100, 0); //keep
  // sscanf(request, "%s %s %s", get, path, http);     //keep
  // newpath = path + 1;                               //keep
  // sprintf(filepath,"%s", newpath);                  //Same as newpath
  // sprintf(header, "Date: %sHostname: %s:%d\nLocation: %s\nContent-Type: %s\nConnection: %s\n\n", asctime(timeinfo), host, PORT, newpath, contentType, connection);
  // if ((fileptr = fopen(filepath, "r")) == NULL)
  // {
  //  printf("File not found!\n");
  //  send(((thread_data *)data)->sd, http_not_found, strlen(http_not_found), 0); //sends HTTP 404
  //  close(((thread_data *)data)->sd);
  // } else {
  //  printf("Sending the file...\n");
  //  send(((thread_data *)data)->sd, http_ok, strlen(http_ok), 0); //sends HTTP 200 OK
  //  send(((thread_data *)data)->sd, header, strlen(header), 0); //sends the header
  //  memset(&buffer, 0, sizeof(buffer));
  //  while (!feof(fileptr)) //sends the file
  //  {
  //   fread(&buffer, sizeof(buffer), 1, fileptr);
  //   send(((thread_data *)data)->sd, buffer, sizeof(buffer), 0);
  //   memset(&buffer, 0, sizeof(buffer));
  //  }
  //  printf("File sent...\n");
  //  close(((thread_data *)data)->sd);
  // }

  // Initializing char pointers
  char *header, *request, *path, *newpath, *host, *temp;
  char get[3], http[9];
  FILE *filePtr;
  char filepath[BUF_SIZE];
  char buffer[BUF_SIZE]; //discard if find size of file

  // Allocating memory size of the arrays
  header = (char*)malloc(BUF_SIZE*sizeof(char));
  request = (char*)malloc(BUF_SIZE*sizeof(char));
  path = (char*)malloc(BUF_SIZE*sizeof(char));
  newpath = (char*)malloc(BUF_SIZE*sizeof(char));

  // Extract headers and parse the first row
  recv(((thread_data *)data)->sd, request, 100, 0);
  sscanf(request, "%s %s %s", get, path, http);
  newpath = path + 1; //Eliminate the extra slash in order to locate file properly

  stringstream ss;
  string response;
  sprintf(filepath,"%s", newpath);
  if ((temp = strstr(filepath, "../")) != NULL)
  {
    ss << "HTTP/1.1 403 Forbidden\r\n\r\n";
    response = ss.str();
    send(((thread_data *)data)->sd, response.c_str(), strlen(response.c_str()), 0);
    close(((thread_data *)data)->sd);
    cout << "\nForbidden request made. Closing connection to client..." << endl;
  }
  if ((temp = strstr(filepath, "SecretFile.html")) != NULL)
  {
    ss << "HTTP/1.1 401 Unauthorized\r\n\r\n";
    response = ss.str();
    send(((thread_data *)data)->sd, response.c_str(), strlen(response.c_str()), 0);
    close(((thread_data *)data)->sd);
    cout << "\nUnauthorized request made. Closing connection to client..." << endl;
  }
  if ((temp = strstr(get, "GET")) == NULL)
  {
    ss << "HTTP/1.1 400 Bad Request\r\n\r\n";
    response = ss.str();
    send(((thread_data *)data)->sd, response.c_str(), strlen(response.c_str()), 0);
    close(((thread_data *)data)->sd);
    cout << "\nBad request made. Closing connection to client..." << endl;
  }
  if ((filePtr = fopen(newpath, "r")) == NULL)
  {
    ss << "HTTP/1.1 404 Not Found\r\n\r\n";
    response = ss.str();
    send(((thread_data *)data)->sd, response.c_str(), strlen(response.c_str()), 0);
    close(((thread_data *)data)->sd);
    cout << "\nFile does not exist. Closing connection to client..." << endl;
  }
  else
  {
    ss << "HTTP/1.1 200 OK\r\n"
       << "Content-Type: text/html\r\n"
       << "Connection: close\r\n"
       << "\r\n";
    response = ss.str();
    send(((thread_data *)data)->sd, response.c_str(), strlen(response.c_str()), 0);
    memset(&buffer, 0, sizeof(buffer));
    while (!feof(filePtr)) //sends the file
    {
     fread(&buffer, sizeof(buffer), 1, filePtr);
     send(((thread_data *)data)->sd, buffer, sizeof(buffer), 0);
     memset(&buffer, 0, sizeof(buffer));
    }
    close(((thread_data *)data)->sd);
    cout << "\nHTML file sent. Closing connection to client..." << endl;
  }
//*****************************************************************************
  // Check path for SecretFile.html (401), ../ (403)
  //1: Open File, 2: Find Size of File, 3: Write Response
  // FILE * fileptr;
  // path = path.substr(1);
  // const char * pathChar = path.c_str();
  // if ((fileptr = fopen(pathChar, "r")) == NULL)
  // {
  //   cout << "File not found." << endl;
  // }
  // else
  // {
  //   char okStatus[] = "HTTP/1.1 200 OK\r\n"
  //                     "Content-Type: text/html\r\n"
  //                     "Connection: close\r\n"
  //                     //"Content-Length: \r\n"
  //                     "\r\n";
  //   memset(&buffer, 0, sizeof(buffer));
  //   while (!feof(fileptr))
  //   {
  //     fread(&buffer, sizeof(buffer), 1, fileptr);
  //     send(((thread_data *)data)->sd, buffer, sizeof(buffer), 0);
  //     memset(&buffer, 0, sizeof(buffer));
  //   }
  // }
// *********************************************************************************
  // ifstream page(path);
  // if (page)
  // {
  //   cout << "200 OK" << endl;
  // }
  // else
  // {
  //   string alter = path.substr(1);
  //   ifstream page2(alter);
  //   if (page2)
  //   {
  //     cout << "200 OK" << endl;
  //   }
  // }
  // while (true)
  // {
  //   int bytes_read = read(page, buffer, sizeof(buffer));
  //   if (bytes_read == 0)
  //   {
  //     break;
  //   }
  //   void *ptr = buffer;
  //   while (bytes_read > 0)
  //   {
  //     int bytes_written = write(((thread_data *)data)->sd, ptr, bytes_read);
  //     bytes_read -= bytes_written;
  //     ptr += bytes_written;
  //   }
  // }
  //write(((thread_data *)data)->sd, content, sizeof(content));
  cout << newpath << endl;
} // end of parsePath

//----------------------------------main----------------------------------------
//Description: The primary driver for the server. It creates the initial socket
//             via the server port number being passed in as an argument,
//             then listens for a connecting host and establishes threads for
//             each connection.
//------------------------------------------------------------------------------

int main ()
{
  int serverSd;                  // Declare a server socket descriptor
  sockaddr_in acceptSockAddr;    // Declare a sockaddr_in structure

  bzero((char*)&acceptSockAddr, sizeof(acceptSockAddr)); // Zero initialize it
  // Set its data members
  acceptSockAddr.sin_family       = AF_INET; // Address Family Internet
  acceptSockAddr.sin_addr.s_addr  = htonl(INADDR_ANY);
  acceptSockAddr.sin_port         = htons(PORT);

  // Open a stream-oriented socket with the internet address Family
  if ((serverSd = socket(AF_INET, SOCK_STREAM, 0)) <= -1)
  {
    perror("Unable to open a server socket.");
  }

  // Set the SO_REUSEADDR option
  const int on = 1;
  if (setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) <= -1)
  {
    perror("Unable to execute setsockopt SO_REUSEADDR.");
  }

  // Bind this socket to its local Address
  if (bind(serverSd, (sockaddr*)&acceptSockAddr, sizeof(acceptSockAddr)) <= -1)
  {
    perror("Unable to bind the server socket to the local address.");
    exit(1);
  }

  // Instruct the operating system to listen to up to n connection requests
  listen(serverSd, 10);

  cout << "Server started and listening on port " << PORT << "..." << endl;

  // Receive a request from a client
  int newSd; // Declare new serverSd
  sockaddr_in newSockAddr;
  socklen_t newSockAddrSize = sizeof(newSockAddr);

  struct thread_data *data; // Declare thread_data struct pointer
  while (1)
  {
    if ((newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize)) <= -1)
    {
      perror("Unable to accept incoming socket connection.");
    }
    cout << "Accepted inbound connection from client..." << endl;

    // Create new thread
    pthread_t new_thread;
    data              = new thread_data;
    data->sd          = newSd;
    int iret1         = pthread_create(&new_thread, NULL, parsePath, (void*) data);
  }

  // Close socket and free memory
  close(data->sd);

  return 0;
}
