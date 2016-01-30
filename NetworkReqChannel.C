#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "NetworkReqChannel.H"

#define MAX_MESSAGE 256

// Client-side
NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no) {
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[MAX_MESSAGE];
    
    //Create a TCP socket
    c_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (c_socket < 0) 
        perror("ERROR: Client socket could not be created");
    
    //Lookup function that returns IP address
    server = gethostbyname(_server_host_name.c_str());
    if (server == NULL) {
        perror("ERROR: No such host");
        exit(EXIT_FAILURE);
    }
    
    //Fill addr with zeros
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(server->h_addr, &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(_port_no);
   
    // Connect TCP socket
    if(connect(c_socket,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR: Unable to connect to server");
        exit(EXIT_FAILURE);
    }
}

// Server-side
NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, const int _backlog, void * (*connection_handler) (void*)) {
    int new_socket, pid;
    socklen_t len;
    struct sockaddr_in server_addr, client_addr;
    
    // Make a TCP Socket 
    s_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(s_socket < 0) {
        perror("ERROR: Server socket could not be created");
        exit(EXIT_FAILURE);
    }
    
    //Fill with zeros
    memset(&server_addr, 0, sizeof(server_addr));
    
    //Setup addressing structs
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port_no);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(s_socket, (struct sockaddr*) &server_addr,  sizeof(server_addr)) < 0) {
        perror("ERROR: Unable to bind server socket");
        exit(EXIT_FAILURE);
    }
    
    // Start listening for incoming clients
    listen(s_socket, _backlog);
    
    len = sizeof(client_addr);
    while (true) {
    //Accept new connection and assign to new filedescriptor
        new_socket = accept(s_socket, (struct sockaddr*) &client_addr, &len);
        
        if(new_socket < 0) {
            perror("ERROR: Unable to accept new client");
            exit(EXIT_FAILURE);
        }
        
        // Fork the process to handle the connection
        pid = fork();
        
        if(pid < 0)
            perror("ERROR: Unable to fork");
        
        // If inside child process...
        if(pid == 0) {
            connection_handler(&new_socket);
        } 
        
        // Close the new socket
        close(new_socket); 
    }
}

NetworkRequestChannel::~NetworkRequestChannel() {
    close(s_socket);
    close(c_socket);
}

string NetworkRequestChannel::send_request(string _request) {
    cwrite(_request);
    string s = cread();
    return s;
}

string NetworkRequestChannel::cread() {
    char buf[MAX_MESSAGE];

    if(read(c_socket, buf, MAX_MESSAGE) < 0)
        perror("ERROR: Unable to read message");
    
    string s = buf;
    return s;
}

int NetworkRequestChannel::cwrite(string _msg) {
    if(_msg.length() >= MAX_MESSAGE) {
        cerr << "Message too long!\n";
        return -1;
    }
    
    const char * s = _msg.c_str();
    
    if(write(c_socket, s, strlen(s) + 1) < 0)
       perror("ERROR: Unable to write message");
}
