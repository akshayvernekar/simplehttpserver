/*
 * httpserver.cpp
 *
 *  Created on: Mar 19, 2019
 *      Author: akki
 */

#include "httpserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sstream>

#define NUMBER_OF_CLIENTS	5
#define BUFFERSIZE	1024

static char* bad_request_response = "\
HTTP/1.1 400 Bad Request\n \
Content-type: text/html\n \
\n \
<html>\n \
 <body>\n \
 <h1>Bad Request</h1>\n\
 <p>This server did not understand your request.</p>\n \
 </body>\n \
</html>\n";

static char* ok_response = "\
HTTP/1.1 200 OK\n \
Content-type: text/html\n \
\n";

Httpserver::Httpserver()
{
	// TODO Auto-generated constructor stub

}

Httpserver::~Httpserver()
{
	// TODO Auto-generated destructor stub
}

void Httpserver::handleClient(int client_fd)
{
	if(client_fd == NULL)
	{
		return ;
	}

	char buffer[BUFFERSIZE] = {0};
	int readBytes = read(client_fd,buffer,BUFFERSIZE);
	std::string httpVer, httpMethod, httpurl;
	std::istringstream stream;
	int writeBytes=0;
	if(readBytes > 0)
	{
		buffer[readBytes] = '\0';
		stream.str(buffer);
		std::string line ;

//		//TODO: extreact all the headers
//		while(std::getline(stream,line))
//		{
//			std::cout << "Line :" << line << std::endl;
//		}

		stream >> httpMethod  ;
		stream >> httpurl;
		stream >> httpVer;

		std::cout << "Method: " << httpMethod<< " ,Version :"<<httpVer << std::endl;
		if(httpVer.compare("HTTP/1.0") && httpVer.compare("HTTP/1.1"))
		{
			std::cout << "unsupported version " << std::endl;

			writeBytes = write(client_fd,bad_request_response,sizeof(bad_request_response));
			std::cout << "wrote " << writeBytes << " Bytes" <<std::endl;
		}
		else
		{
			if(!httpMethod.compare("GET"))
			{
				if(mGetHandle != NULL)
				{
					char* data = mGetHandle(NULL);
					if(data != NULL)
					{
						//write header
						writeBytes = write(client_fd,ok_response,strlen(ok_response));
						std::cout << "wrote " << writeBytes << " Bytes" <<std::endl;
						//write body
						writeBytes = write(client_fd,data,strlen(data));
						std::cout <<"input size " << strlen(data)<< "data : "<< data<< "wrote " << writeBytes << " Bytes" <<std::endl;
					}
					else
						write(client_fd,bad_request_response,sizeof(bad_request_response));
				}
			}
			else
			{
				std::cout << "unsupported method " << std::endl;
				write(client_fd,bad_request_response,sizeof(bad_request_response));
			}

		}
	}

	close(client_fd);

}

int Httpserver::startServer(int port)
{
	int server_fd, cli_socket, valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );

	if (bind(server_fd, (struct sockaddr *)&address,
								 sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		if (listen(server_fd, NUMBER_OF_CLIENTS) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}

		if ((cli_socket = accept(server_fd, (struct sockaddr *)&address,
						   (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		 //write(cli_socket,ok_response,strlen(ok_response));
		handleClient(cli_socket);

	}

	return 0;
}

void Httpserver::addHandle(HTTP_METHOD method,char* (*handle)(void*))
{
	switch (method)
	{
	case eGET:
		mGetHandle = handle;
		break;
	case ePUT:
		break;
	case ePOST:
		break;
	default:
		std::cout << "unsupported HTTP method" << std::endl;
	}
}


