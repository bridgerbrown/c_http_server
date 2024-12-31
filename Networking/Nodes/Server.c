#include "Server.h"
#include <stdio.h>
#include <stdlib.h>

struct Server server_constructor(int domain, int service, int protocol, 
		u_long interface, int port, int backlog, void (*launch)(struct Server *server))
{
	// 1. Creates server object then returns to whoever called it
	// 2. Uses header parameters to determine them for given address
	// 3. Creates a socket for the server, bind socket, and starts listening
	struct Server server;
	
	server.domain = domain;
	server.service = service;
	server.protocol = protocol;
	server.interface = interface;
	server.port = port;
	server.backlog = backlog;

	server.address.sin_family = domain; // 
	server.address.sin_port = htons(port); // convert int port to bytes that will refer to a network port
	server.address.sin_addr.s_addr = htonl(interface); // 
	
	// Create socket for server
	server.socket = socket(domain, service, protocol); // create socket connection to network
	if (server.socket == 0)
	{
			perror("Failed to connect socket...\n");
			exit(1);
	}

	// Bind socket to network
	if (bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) < 0)
	{
			perror("Failed to bind socket...\n");
			exit(1);
	}
	
	if (listen(server.socket, server.backlog) < 0)
	{
			perror("Failed to start listening...\n");
			exit(1);
	}

	server.launch = launch;

	return server;
}
