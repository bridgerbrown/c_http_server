#include "HTTPServer.h"
#include "../../Systems/ThreadPool.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// PUBLIC MEMBER FUNCTIONS
void launch(struct HTTPServer *server);
void * handler(void *arg);

// PUBLIC HELPER FUNCTIONS
void register_routes(struct HTTPServer *server, 
										 char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), 
										 char *uri, int num_methods, ...);

// PRIVATE DATA TYPES
struct ClientServer
{
		int client;
		struct HTTPServer *server;
};

struct Route
{
		int methods[9];
		char *uri;
		char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request);
};

// CONSTRUCTORS
struct HTTPServer http_server_constructor()
{
		struct HTTPServer server;
		server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 255);
		server.routes = dictionary_constructor(compare_string_keys);
		server.register_routes = register_routes;
		server.launch = launch;
		return server;
}

void register_routes(struct HTTPServer *server,
										 char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request),
										 char *uri, int num_methods, ...)
{
		struct Route route;
		va_list methods;
		va_start(methods, num_methods);
		for (int i = 0; i < num_methods; i++)
		{
				route.methods[i] = va_arg(methods, int);
		}
		// register uri
		char buffer[strlen(uri)];
		route.uri = buffer;
		strcpy(route.uri, uri);
		route.route_function = route_function;
		server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct HTTPServer *server)
{
		// initialize thread pool to handle clients
		struct ThreadPool thread_pool = thread_pool_constructor(20);
		struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
		socklen_t address_length = (socklen_t)sizeof(server->server.address);
		// keep server alive
		while (1)
		{
				struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
				// accept incoming connection
				client_server->client = accept(server->server.socket, sock_addr, &address_length);
				client_server->server = server;
				// pass client to thread pool
				struct ThreadJob job = thread_job_constructor(handler, client_server);
				thread_pool.add_work(&thread_pool, job);
		}
}

// PRIVATE MEMBER FUNCTIONS
void * handler(void *arg)
{
		struct ClientServer *client_server = (struct ClientServer *)arg;
		// read client request
		char request_string[30000];
		read(client_server->client, request_string, 30000);
		struct HTTPRequest request = http_request_constructor(request_string);
		char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
		struct Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)]));
		// process request and respond to client
		char *response = route->route_function(client_server->server, &request);
		write(client_server->client, response, sizeof(char[strlen(response)]));
		close(client_server->client);
		free(client_server);
		// cleanup request
		http_request_destructor(&request);
		return NULL;
}

// PUBLIC HELPER FUNCTIONS
char *render_template(int num_templates, ...)
// joins contents of multiple files into one
{
		char *buffer = malloc(30000);
		int buffer_position = 0;
		char c;
		FILE *file;

		va_list files;
		va_start(files, num_templates);

		for (int i = 0; i < num_templates; i++)
		{
				char *path = va_arg(files, char*);
				file = fopen(path, "r");
				while ((c = fgetc(file)) != EOF)
				{
						buffer[buffer_position] = c;
						buffer_position += 1;
				}
		}
		va_end(files);
		return buffer;
}
