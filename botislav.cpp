

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <map>
//colors!!!!!!!!
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define WHITE "\033[0;37m" 
#define YELLOW "\033[0;33m"
#define SHALLOW "\033[0m"
#define BRED "\033[7;31m"
#define BGBLUE "\033[44m"
//colors!!!!!!!!

std::map < std::string, int > names;
bool work = true;
int sock_sig = 0;
// :b!b@127.0.0.1 PRIVMSG a : Hello
void sig_handler(int signal)
{
	(void)signal;
	work = false;
	send(sock_sig, "QUIT\r\n", 6, 0);
}

void auth(int sock, const std::string &pass)
{
	std::string pass_("PASS "), nick_("NICK bot\r\n"), user_("USER a a a :a\r\n");

	if (send(sock, (pass_ + pass + "\r\n").c_str(), (pass_ + pass + "\r\n").length(), 0) < 0)
	{
		std::cout << "Could not auth. Pass error" << std::endl;
		exit (1);
	}
	if (send(sock, user_.c_str(), user_.length(), 0) < 0)
	{
		std::cout << "Could not auth. User error" << std::endl;
		exit (1);
	}
	if (send(sock, nick_.c_str(), nick_.length(), 0) < 0)
	{
		std::cout << "Could not auth. Nick error" << std::endl;
		exit (1);
	}
	char bufread[512];
	memset(bufread, 0, 512);
	int res = recv(sock, bufread, 512, 0);
	if (res <= 0) {
		std::cout << "There is no connection to server"<< std::endl;
		exit (1);
	}
	std::cout << RED << "auth" << SHALLOW << std::endl;
	std::string frombuf(bufread);
	std::string m("Welcome to the Internet");
	std::string l("Password incorrect");
	std::cout << RED  << frombuf << SHALLOW << std::endl;
	if (frombuf.find(l) != std::string::npos){
		std::cout << "Sorry, password is incorrect!\n";
		exit (1);
	}
	if (frombuf.find(m) == std::string::npos){
		std::cout << "Sorry, but user bot is already exists!\n";
		exit (1);
	}
}

void draw_hello(int sock, std::string const &name){
	std::string s1("PRIVMSG " + name + " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" + "\r\n");
	std::string s2("PRIVMSG " + name + " ░░░█░░░█░░░░░█████░░░░█░░░░░░░█░░░░░░██████░░░░░░░░░" + "\r\n");
	std::string s3("PRIVMSG " + name + " ░░░█░░░█░░░░░█░░░░░░░░█░░░░░░░█░░░░░░░█░░░░█░░░░░░░░" + "\r\n");
	std::string s4("PRIVMSG " + name + " ░░░█████░░░░░████░░░░█░░░░░░░█░░░░░░░█░░░░█░░░░░░░░░" + "\r\n");
	std::string s5("PRIVMSG " + name + " ░░░█░░░█░░░░░█░░░░░░░░█░░░░░░░█░░░░░░░█░░░░█░░░░░░░░" + "\r\n");
	std::string s6("PRIVMSG " + name + " ░░░█░░░█░░░░░█████░░░░█████░░█████░░░██████░░░░░░░░░" + "\r\n");
	std::string s7("PRIVMSG " + name + " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" + "\r\n");
	
	std::string s8("PRIVMSG " + name + " █░░░░░░░█░███████░░░████░░███████░░░░█░░░█░░░░██████░░░░█████░░░" + "\r\n");
	std::string s9("PRIVMSG " + name + " █░░░░░░░█░█░░░░░░░░░█░░░█░░░░░█░░░░░░░░█░░░█░░░░█░░░░░░░░░█░░░░█░░" + "\r\n");
	std::string s10("PRIVMSG " + name + " █░░░█░░░█░█████░░░░█████░░░░░█░░░░░░░░█████░░░░████░░░░░░████░░░░" + "\r\n");
	std::string s11("PRIVMSG " + name + " █░░█░█░░█░█░░░░░░░░█░░░░█░░░░░█░░░░░░░░█░░░█░░░░█░░░░░░░░░█░░░░█░░" + "\r\n");
	std::string s12("PRIVMSG " + name + " ░███░███░░███████░░█░░░█░░░░░█░░░░░░░░█░░░░█░░░██████░░░░█░░░░░█░░" + "\r\n");
	std::string s13("PRIVMSG " + name + " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" + "\r\n");


	send(sock, s1.c_str(), s1.length(), 0);
	send(sock, s2.c_str(), s2.length(), 0);
	send(sock, s3.c_str(), s3.length(), 0);
	send(sock, s4.c_str(), s4.length(), 0);
	send(sock, s5.c_str(), s5.length(), 0);
	send(sock, s6.c_str(), s6.length(), 0);
	send(sock, s7.c_str(), s7.length(), 0);
	send(sock, s8.c_str(), s8.length(), 0);
	send(sock, s9.c_str(), s9.length(), 0);
	send(sock, s10.c_str(), s10.length(), 0);
	send(sock, s11.c_str(), s11.length(), 0);
	send(sock, s12.c_str(), s12.length(), 0);
	send(sock, s13.c_str(), s13.length(), 0);

}

void draw_msg(int sock, std::string const &name, std::string const &msg)
{
	std::string s1("PRIVMSG " + name + " :" + msg );
	send(sock, s1.c_str(), s1.length(), 0);
}

void hello_msg(int sock, std::string const &name){
	std::string s10("PRIVMSG " + name + "\r\n");
	send(sock, s10.c_str(), s10.length(), 0);
}

std::string extract_nick(std::string &message)
{
	std::string ret;
	for (std::string::iterator It = message.begin() ; It != message.end(); ++It)
	{
		if (*It == '!')
		{
			ret = std::string(++message.begin(), It);
			break ;
		}
	}
	return (ret);
}

// :b!b@127.0.0.1 PRIVMSG a :Hello
std::string extract_msg(std::string &message)
{
	std::cout << RED << "message: " << message << SHALLOW << std::endl;
	std::string ret;
	std::string ret1;
	std::string::iterator It;
	size_t flag = 0;
	for (It = message.begin() ; It != message.end(); ++It)
	{
		if (*It == ':')
		{
			if (!flag){
				It++;
				flag = 1;
			}
			else{
				It++;
				ret = std::string(It, message.end());
				break;
			}
		}
	}
	return (ret);
}

int get_socket_weather()
{
	char buf[1024];
	int sock_w;
	struct sockaddr_in addr;
	sock_w = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_w < 0)
		exit (1);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_aton("5.9.243.187", &(addr.sin_addr));
	if(connect(sock_w, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		exit (2);
	return (sock_w);
}


void message_processing(int sock, std::string &message, int sock_w)
{
	
	if (message.find("PRIVMSG") == std::string::npos)
		return ;
	std::string name = extract_nick(message);
	std::string msg = extract_msg(message);
	std::pair<std::map <std::string, int >::iterator,bool> ret;
	ret = names.insert ( std::pair<std::string, int >(name, 1) );
	if (ret.second==true) {
		draw_hello(sock, name);
		draw_msg(sock, name, ":Hello, my friend! 😀 I am a weather bot. I can tell you the WEATHER. ☂ You shoud type the name of the \"city\" withour white spaces. If you will JOKE with me, I will JOKE with you 🤡\r\n");
	}
	if (ret.second==false) {
		draw_msg(sock, name, "...🤔...\r\n");

		std::string place = msg.substr(0, msg.size() - 2);
		char buf[1024];
		memset(buf, 0, 1024);
		std::string request = "GET /" + place + "?format=3 HTTP/1.1\nUser-Agent: curl/7.29.0\nHost: wttr.in\n\n";
		std::cout << RED <<  "request: " << request <<  SHALLOW<< std::endl;
		int status = send(sock_w, request.c_str(), request.length(), 0);
		if (status != request.length()) {
			std::cout << "Request error!\n";
			return ;
		}
		recv(sock_w, buf, 1024, 0);
		std::cout << GREEN << "buf:" <<  buf << SHALLOW << std::endl;
		std::string rezult = buf;
		rezult.pop_back();
		rezult = rezult.substr(rezult.find_last_of('\n') + 1, rezult.length());
		std::cout << YELLOW << "result: " << rezult   <<  SHALLOW<< std::endl;
		rezult += "\r\n";
		draw_msg(sock, name, rezult);
		draw_msg(sock, name, ":Please type the name of the \"city\" without white spaces. If you will JOKE with me, I will JOKE with you 🤡\r\n");
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./bot <port> <password>" << std::endl;
		exit (0);
	}
	int sock;
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	int opt = 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		return 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if(connect(sock, (sockaddr *)&server, sizeof(server))< 0)
		return 2;
	
	// fcntl(sock, F_SETFL, O_NONBLOCK);
	auth(sock, std::string(argv[2]));
	char buf[512];

	signal(SIGINT, sig_handler);
	sock_sig = sock;
	std::string message;
	names.clear();

	int sock_w = get_socket_weather();
	while (work)
	{	
		memset(buf, 0, 512);
		message.clear();
		if (recv(sock, buf, 512, 0) <= 0)
		{
			std::cout << "There is no connection to server"<< std::endl;//??????
			break;
		}
		message = buf;
		int ret;
		message_processing(sock, message, sock_w);
	}
	close(sock_w);
	close(sock);
}

