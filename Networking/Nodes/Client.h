#ifndef Client_h
#define Client_h

#include <sys/socket.h>
#include <netinet/in.h>

// DATA TYPES
struct Client
{
		// PUBLIC MEMBER VARIABLES 
		int socket;
		int domain;
		int service;
		int protocol;
		int port;
		u_long interface;
		// PUBLIC MEMBER METHODS
		// allows a client to request of a specified server
		char * (*request)(struct Client *client, char *server_ip, void *request, unsigned long size);
};

// CONSTRUCTORS
struct Client client_constructor(int domain, int service, int protocol, int port, u_long interface);
#endif
