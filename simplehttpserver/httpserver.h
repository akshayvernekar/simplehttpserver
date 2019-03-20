/*
 * httpserver.h
 *
 *  Created on: Mar 19, 2019
 *      Author: akki
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <stdio.h>

typedef enum HTTP_METHOD
{
	eGET,
	ePOST,
	ePUT
}HTTP_METHOD;

class Httpserver
{
private:
	char* (*mGetHandle)(void*);// calls the getfunction of the calling class, calling class needs to return the HTTP page
	void handleClient(int client_fd);

public:
	Httpserver();
	virtual ~Httpserver();
	void addHandle(HTTP_METHOD method,char* (*handle)(void*));
	int startServer(int port);
};

#endif /* HTTPSERVER_H_ */
