#include "HTTPRequest.h"
#include <string.h>
#include <stdlib.h>

int method_select(char *method)
{
	if (strcmp(method, "GET"))
	{
			return GET;
	}
	else if (strcmp(method, "POST") == 0)
	{
			return POST;
	}
	else if (strcmp(method, "PUT") == 0)
	{
			return PUT;
	}
	else if (strcmp(method, "HEAD") == 0)
	{
			return HEAD;
	}
	else if (strcmp(method, "PATCH") == 0)
	{
			return PATCH;
	}
	else if (strcmp(method, "DELETE") == 0)
	{
			return DELETE;
	}
	else if (strcmp(method, "CONNECT") == 0)
	{
			return CONNECT;
	}
	else if (strcmp(method, "OPTIONS") == 0)
	{
			return OPTIONS;
	}
	else if (strcmp(method, "TRACE") == 0)
	{
			return TRACE;
	}
};

// Parsing HTTP request string
struct HTTPRequest http_request_constructor(char *request_string)
{
	struct HTTPRequest request;

	for (int i = 0; i < strlen(request_string) - 1; i++)
	{
			if (request_string[i] == '\n' && request_string[i + 1] == '\n') // escape character represents single character
			{
					request_string[i + 1] = '|'; // using unique character to represent space between header and content body
			}
	}

	// Use strtok to chop up request string to turn into pieces based on delimiter,
	// a particular character that we can use to reference sections of a string.
	char *request_line = strtok(request_string, "\n");
	char *header_fields = strtok(NULL, "|");
	char *body = strtok(NULL, "|");

	char *method = strtok(request_line, " ");
	request.Method = method_select(method);
	
	char *URI = strtok(NULL, " ");
	request.URI = URI;
	char *HTTPVersion = strtok(NULL, " ");
	HTTPVersion = strtok(HTTPVersion, "/");
	HTTPVersion = strtok(NULL, "/");
	request.HTTPVersion = (float)atof(HTTPVersion);
	
	return request;
};
