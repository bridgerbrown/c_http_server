#ifndef HTTPRequest_h
#define HTTPRequest_h

#include <stdio.h>

enum HTTPMethods
{
		GET,
		POST,
		PUT,
		HEAD,
		PATCH,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE
};

struct HTTPRequest
{
		int Method;
		char *URI; // pointer since we don't know length
		float HTTPVersion;
};

struct HTTPRequest http_request_constructor(char *request_string);

#endif
