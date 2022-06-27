#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "client.hpp"
#include <vector>

class Channel {
public:
	typedef std::vector<Client>::const_iterator client_const_it;
	typedef std::vector<const Client*>::iterator client_point_it;	
	typedef std::vector<const Client*>::const_iterator client_const_point_it;
private:
	std::string					_name;
	std::string					_topic;
	std::string					_mode;
	std::vector<const Client*> 	_clients;
	std::vector<const Client*> 	_invite;
public:
	Channel(std::string& name, client_const_it& it);
	void add_in_channel(client_const_it& it);
	void remove_from_channel(const std::string &name);
	void send_in_channels(std::string str, client_const_it it, bool ret_message);
	void put_in_topic(std::string& t, client_const_it it);
	short put_in_mode(std::string& m, client_const_it it);
	short kick_from_channel(std::string& n, client_const_it it);
	bool find_mode(char c) const;
	bool find_nick_in_channel(const std::string& nick);
	bool check_operator(client_const_it it);
	void put_invite(client_const_it it);
	void erase_invite(client_const_it it);
	bool find_invite(client_const_it it);
	void fix_addr_offset(Client *delete_addr);

	const std::string get_names_users() const;
	const std::string& get_name_channel() const;
	const std::string& get_topic() const;
	const std::string get_topic_message(client_const_it it) const;
	const std::string get_names_message(client_const_it it) const;
	const int get_count_users() const;
};

#endif