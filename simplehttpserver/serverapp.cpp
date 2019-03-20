#include <iostream>
#include <stdio.h>
#include "httpserver.h"

static char* response="<!DOCTYPE html> \n \
<html>\n \
 <body>\n \
 <h1>Welcome</h1>\n\
 <p>Welcome to the server .</p>\n \
 </body>\n \
</html>\n";

char* getmethod(void*)
{
	std::cout << "getmethod::we were hit" << std::endl;
	return response;
}

int main()
{
	Httpserver httpserver;
	httpserver.addHandle(eGET,getmethod);
	std::cout << "running the server" << std::endl;
	httpserver.startServer(8080);

}
