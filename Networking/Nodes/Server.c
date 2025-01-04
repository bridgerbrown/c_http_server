#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void register_routes_server(struct Server *server, char *(*route_function)(void *arg), char *path);

// CONSTRUCTORS
struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog) 
{
	struct Server server;
	
	server.domain = domain;
	server.service = service;
	server.protocol = protocol;
	server.interface = interface;
	server.port = port;
	server.backlog = backlog;

	// socket address structure
	server.address.sin_family = domain; 
	server.address.sin_port = htons(port); // convert int port to bytes that will refer to a network port
	server.address.sin_addr.s_addr = htonl(interface); 
	
	// create socket for server
	server.socket = socket(domain, service, protocol); // create socket connection to network
	
	server.routes = dictionary_constructor(compare_string_keys);
	server.register_routes = register_routes_server;

	if (server.socket == 0)
	{
			perror("Failed to connect socket...\n");
			exit(1);
	}

	// bind socket to network
	if (bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) < 0)
	{
			perror("Failed to bind socket...\n");
			exit(1);
	}
	// listen on network	
	if (listen(server.socket, server.backlog) < 0)
	{
			perror("Failed to start listening...\n");
			exit(1);
	}

	return server;
}

void register_routes_server(struct Server *server, char *(route_function)(void *arg), char *path)
{
	struct ServerRoute route;
	route.route_function = route_function;
	server->routes.insert(&server->routes, path, sizeof(char[strlen(path)]), &route, sizeof(route));
}
