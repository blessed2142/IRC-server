#ifndef HANDLE_COMMAND_HPP
#define HANDLE_COMMAND_HPP

#include "client.hpp"
#include "server.hpp"
#include <vector>
#include <string>

class Server;

class Handle_command {
	Server*							_server;
	std::vector<Client>::iterator	_it;
	std::string						_prefix;
	std::string						_command;
	std::vector<std::string>		_parametrs;

	std::string do_for_login(void);
	std::string put_in_answer(std::string message);
	std::string welcome(void);
	void execute(void);
	void sendd(int sock, std::string str);
	void quit(void);
	void privmsg(bool flag); // NOTICE=0, PRIVMSG=1
	void oper(void);
	void kill(void);
	void restart(void);
	void join(void);
	void part(void);
	void mode(void);
	void topic(void);
	void invite(void);
	void kick(void);
	void who(void);
	void list(void);
	void ping(void);

	std::string create_priv_message(std::string& name, std::string& mes, bool flag);
	std::string create_welc_message();
	void send_motd();
	void create_channels(std::string& tmp);
	void join_in_channels(std::string& tmp);
	void send_channel_list(std::string& tmp);
	void send_topic(std::string& tmp);
public:
	Handle_command(std::vector<Client>::iterator &i, std::string comm, Server* s);
	void handle_exec(void);
};

#endif
