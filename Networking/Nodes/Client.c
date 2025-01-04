#include "Client.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

// FUNCTION PROTOTYPES
char * request(struct Client *client, char *server_ip, void *request, unsigned long size);

// CONSTRUCTORS
struct Client client_constructor(int domain, int service, int protocol, int port, u_long interface)
{
	struct Client client;
	client.domain = domain;
	client.port = port;
	client.interface = interface;
	client.socket = socket(domain, service, protocol);
	client.request = request;
	return client;
}

// PRIVATE MEMBER METHODS
char * request(struct Client *client, char *server_ip, void *request, unsigned long size)
{
	struct sockaddr_in server_address;
	server_address.sin_family = client->domain;
	server_address.sin_port = htons(client->port);
	server_address.sin_addr.s_addr = (int)client->interface;

	inet_pton(client->domain, server_ip, &server_address.sin_addr);
	connect(client->socket, (struct sockaddr*)&server_address, sizeof(server_address));
	send(client->socket, request, size, 0);
	char *response = malloc(30000);
	read(client->socket, response, 30000);
	return response;
}
