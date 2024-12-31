#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"

void launch(struct Server *server)
{
		char buffer[30000];
		char *hello = "HTTP/1.1 200 OK\r\n"
									"Date: Mon, 30 Dec 2024 12:00:00 GMT\r\n"
									"Server: Apache/2.2.14 (Win32)\r\n"
									"Last-Modified: Mon, 30 Dec 2025 12:00:00 GMT\r\n"
									"Content-Length: 39\r\n"
									"Content-Type: text/html\r\n"
									"Connection: Closed\r\n\r\n"
									"<html><body><h1>C Server</h1></body></html>";
				int address_length = sizeof(server -> address);
		int new_socket;

		while (1)
		{
				printf("==== WAITING FOR CONNECTION ====\n");
				new_socket = accept(server -> socket, (struct sockaddr *)&server -> address, (socklen_t *)&address_length);
				read(new_socket, buffer, 30000);
				printf("%s\n", buffer); // wants a char array pointer, this puts a string
				write(new_socket, hello, strlen(hello));
				close(new_socket);
		}
}

int main()
{
		struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
		server.launch(&server);
}
