#include <iostream>
#include "includes/server.hpp"

int main(int argc, const char **argv) {
	if (argc != 3) {
		std::cout << "Use: " << argv[0] << " port password\n";
		return 0;
	}
	int port = std::stoi(argv[1]); // C++ 11 
	if (port < 1024 || port > 65535) {
		std::cout << "1024 < port < 65535\n";
		return 0;
	}
	Server serv(port, argv[2]);
	serv.start();
	while (1) {
		serv.work();
	}
}