#include "HTTPRequest.h"
#include "../../DataStructures/Lists/Queue.h"

#include <string.h>
#include <stdlib.h>

// PRIVATE MEMBER METHODS
void extract_request_line_fields(struct HTTPRequest *request, char *request_line);
void extract_header_fields(struct HTTPRequest *request, char *header_fields);
void extract_body(struct HTTPRequest *request, char *body);

// CONSTRUCTORS
struct HTTPRequest http_request_constructor(char *request_string)
{
	struct HTTPRequest request;
	// convert from string literal and replace blank line with |
	char requested[strlen(request_string)];
	strcpy(requested, request_string);
	for (int i = 0; i < strlen(requested) - 2; i++)
	{
			if (requested[i] == '\n' && requested[i + 1] == '\n') // escape character represents single character
			{
					requested[i + 1] = '|'; // using unique character to represent space between header and content body
			}
	}

	// use strtok to chop up request string to turn into pieces based on delimiter,
	// a particular character that we can use to reference sections of a string.
	char *request_line = strtok(request_string, "\n");
	char *header_fields = strtok(NULL, "|");
	char *body = strtok(NULL, "|");

	// parse each section
	extract_request_line_fields(&request, request_line);
	extract_header_fields(&request, header_fields);
	extract_body(&request, body);

	return request;
};

void http_request_destructor(struct HTTPRequest *request)
{
	dictionary_destructor(&request->request_line);
	dictionary_destructor(&request->header_fields);
	dictionary_destructor(&request->body);
}

// PRIVATE MEMBER METHODS
void extract_request_line_fields(struct HTTPRequest *request, char *request_line)
{
	// copy string literal
	char fields[strlen(request_line)];
	// separate string on spaces for each section
	char *method = strtok(fields, " ");
	char *uri = strtok(NULL, " ");
	char *http_version = strtok(NULL, "\0");
	// insert results into dictionary
	struct Dictionary request_line_dict = dictionary_constructor(compare_string_keys);
	request_line_dict.insert(&request_line_dict, "method", sizeof("method"), method, sizeof(char[strlen(method)]));
	request_line_dict.insert(&request_line_dict, "uri", sizeof("uri"), uri, sizeof(char[strlen(uri)]));
	request_line_dict.insert(&request_line_dict, "http_version", sizeof("http_version"), http_version, sizeof(char[strlen(http_version)]));
	// save dictionary to request object
	request->request_line = request_line_dict;
	if (request->request_line.search(&request->request_line, "GET", sizeof("GET")))
	{
		extract_body(request, (char *)request->request_line.search(&request->request_line, "uri", sizeof("uri")));
	}
}

void extract_header_fields(struct HTTPRequest *request, char *header_fields)
{
	char fields[strlen(header_fields)];
	strcpy(fields, header_fields);
	struct Queue headers = queue_constructor();
	char *field = strtok(fields, "\n");
	while (field)
	{
		headers.push(&headers, field, sizeof(char[strlen(field)]));
		field = strtok(NULL, "\n");
	}

	request->header_fields = dictionary_constructor(compare_string_keys);
	char *header = (char *)headers.peek(&headers);
	while (header)
	{
		char *key = strtok(header, ":");
		char *value = strtok(NULL, "\0");
		if (value)
		{
			if (value[0] == ' ')
			{
				value++;
			}
			request->header_fields.insert(&request->header_fields, key, sizeof(char[strlen(key)]), value, sizeof(char(strlen(value))));
		}
		headers.pop(&headers);
		header = (char *)headers.peek(&headers);
	}
	queue_destructor(&headers);
}

// parses body according to content type in header fields
void extract_body(struct HTTPRequest *request, char *body)
{
	char *content_type = (char *)request->header_fields.search(&request->header_fields, "Content-Type", sizeof("Content-Type"));
	if (content_type)
	{
		struct Dictionary body_fields = dictionary_constructor(compare_string_keys);
		if (strcmp(content_type, "application/x-www-form-urlencoded") == 0)
		{
			// collect each kv pair as a set and store in queue
			struct Queue fields = queue_constructor();
			char *field = strtok(body, "&");
			while (field)
			{
				fields.push(&fields, field, sizeof(char[strlen(field)]));
			}
			field = fields.peek(&fields);
			while (field)
			{
				char *key = strtok(field, "=");
				char *value = strtok(NULL, "\0");
				if (value[0] == ' ')
				{
					value++;
				}
				body_fields.insert(&body_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
				fields.pop(&fields);
				field = fields.peek(&fields);
			}
			queue_destructor(&fields);
		}
		else
		{
			body_fields.insert(&body_fields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
		}
		request->body = body_fields;
	}
}
