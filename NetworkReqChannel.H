#ifndef NETWORK_H
#define NETWORK_H

#include <string>
using namespace std;


class NetworkRequestChannel {
public:
    // The file descriptors for each socket
    int s_socket;
    int c_socket;
    
    /* Creates a CLIENT-SIDE local copy of the channel. The channel is connected
    to the given port number at the given server host.
    THIS CONSTRUCTOR IS CALLED BY THE CLIENT. */
    NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no);
    
    /* Creates a SERVER-SIDE local copy of the channel that is accepting connections
    at the given port number.
    NOTE that multiple clients can be connected to the same server-side end of the
    request channel. Whenever a new connection comes in, it is accepted by the
    the server, and the given connection handler is invoked. The parameter to
    the connection handler is the file descriptor of the slave socket returned
    by the accept call.
    NOTE that the connection handler does not want to deal with
    closing the socket. You will have to close the socket once the
    connection handler is done. */
    NetworkRequestChannel(const unsigned short _port_no, const int _backlog, void* (*connection_handler)(void*));
    
    /* Destructor of the local copy of the channel. */
    ~NetworkRequestChannel();
    
    /* Send a string over the channel and wait for a reply. */
    string send_request(string _request);
    
    /* Blocking read of data from the channel. Returns a string of characters
    read from the channel. Returns NULL if read failed. */
    string cread();
    
    /* Write the data to the channel. The function returns the number of
    characters written to the channel. */
    int cwrite(string _msg);
};

#endif