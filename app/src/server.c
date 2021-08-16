#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include "config.h"
#include "time.h"

static WSADATA wsa;
static SOCKET s;
static struct sockaddr_in server;

extern config_t config;

bool server_init()
{
	// init
	printf("[%s][server] winsock init...\n", time_now());
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		fprintf(stderr, "[server] -> Failed. Error Code : %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

bool server_run()
{
	// create socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		fprintf(stderr, "[server] -> Could not create socket : %d\n" , WSAGetLastError());
		return false;
	}

	// set up 
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(config.host);
	server.sin_port = htons(config.port);

	// bind
	if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		fprintf(stderr, "[server] -> Bind failed with error code : %d\n" , WSAGetLastError());
		return false;
	}

	// listen
	listen(s, 1);
	printf("[%s][server] listening on port %d\n", time_now(), config.port);

	// accept
	int c = sizeof(struct sockaddr_in);
	struct sockaddr_in client;
	SOCKET new_socket = accept(s , (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		fprintf(stderr, "[%s][server] accept failed with error code : %d" , time_now(), WSAGetLastError());
		return false;
	}
	printf("[%s][server] connection accepted from %s\n", time_now(), inet_ntoa(client.sin_addr));

	// request
	int recv_size;
	char request[2000];
	if((recv_size = recv(new_socket, request, 2000 , 0)) == SOCKET_ERROR)
	{
		fprintf(stderr, "[%s][server] recv failed\n", time_now());
		return false;
	}
	request[recv_size] = 0;
	printf("%s\n", request);

	// reply
	const char* reply = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nHello World!";
	send(new_socket, reply, strlen(reply), 0);

	// extra
	while(true)
	{
		if((recv_size = recv(new_socket, request, 2000 , 0)) == SOCKET_ERROR)
		{
			fprintf(stderr, "[%s][server] recv failed\n", time_now());
			return false;
		}
		request[recv_size] = 0;
		printf("%s\n", request);
	}

	// done
	return true;
}

void server_cleanup()
{
	closesocket(s);
	WSACleanup();
}