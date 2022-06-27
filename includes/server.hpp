#ifndef SERVER_HPP
#define SERVER_HPP

#include "client.hpp"
#include "handle_command.hpp"
#include "channel.hpp"
#include <algorithm>

#define SERVER_NAME "Kjaco's_server"
#define BUFFER_SIZE 512
#define TURN_COUNT 10
#define COUNT_CLIENTS 20
#define COUNT_CHANNEL 10

class Server {
public:
	typedef std::vector<Client>::iterator client_it;
	typedef std::vector<Client>::const_iterator client_const_it;
	typedef std::vector<Channel>::iterator channel_it;
	typedef std::vector<Channel>::const_iterator channel_const_it;	
private:
	int								_port;
	const std::string				_password;
	std::vector<Client> 			_clients;
	std::vector<Channel>			_channels;
public:
	Server(int port, std::string password);
	~Server();
	void start();
	void work();
	size_t count_clients() const;
	size_t count_channels() const;
	Client& return_client(size_t index);
	Channel& return_channel(size_t index);
	bool find_nick(const std::string& str) const;
	bool find_chan(const std::string& str) const;
	const std::string get_name_server() const;
	const std::string get_password() const;
	client_it get_client(std::string& name);
	channel_it get_chanel(std::string& name);
	const int get_socket_client(std::string& name) const;
	void create_channels(std::string &name, client_const_it it);
	void add_in_channel(std::string &name, client_const_it it);
	void exit_client(client_it client);
	void exit_channels(client_it client);
	void restart_server();
	void remove_channel(channel_it const &it);
private:
	void reserve_put_vectors(int& sock, struct sockaddr_in& serv);
	void new_client();
	void old_client(client_it &i);
	int put_in_set(fd_set* read_set);
};

#endif

// 1 socket-server create error
// 2 socket-server bind error
// 3 server listen error
// 4 select error
// 5 accept error
// 6 recv error
// 7 send error
// 8 memory allocate error
