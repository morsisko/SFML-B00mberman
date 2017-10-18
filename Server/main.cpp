#include <iostream>
#include "Server.h"

int main()
{
	Server server;
	server.start(25565);
	return 0;
}