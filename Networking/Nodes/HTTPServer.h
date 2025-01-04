#ifndef HTTPServer_h
#define HTTPServer_h

#include "Server.h"
#include "../Protocols/HTTPRequest.h"

// DATA TYPES
struct HTTPServer
{
		// PUBLIC MEMBER VARIABLES
		struct Server server;
		struct Dictionary routes;

		// PUBLIC MEMBER METHODS
		void (*register_routes)(struct HTTPServer *server, char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...);
		void (*launch)(struct HTTPServer *server);
};

enum HTTPMethods
{
		CONNECT,
		DELETE,
		GET,
		HEAD,
		OPTIONS,
		PATCH,
		POST,
		PUT,
		TRACE
};

// CONSTRUCTORS
struct HTTPServer http_request_constructor(void);

// PUBLIC HELPER FUNCTIONS
char *render_template(int num_templates, ...);
#endif
