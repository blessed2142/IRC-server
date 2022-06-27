#include "includes/channel.hpp"
#include "includes/message.hpp"

Channel::Channel(std::string& name, client_const_it& it) : _name(name), _topic(std::string()), _mode(std::string()) {
	_clients.push_back(&(*it));
}

void Channel::add_in_channel(client_const_it& it) {
	_clients.push_back(&(*it));
}

void Channel::remove_from_channel(const std::string &name) {
	client_const_point_it It = _clients.begin();
	client_const_point_it Ite = _clients.end();
	for ( ; It != Ite; ++It) {
		if ((*It)->get_nick() == name)
			break ;
	}
	if (It != Ite) {
		_clients.erase(It);
	}
}

void Channel::send_in_channels(std::string str, client_const_it it, bool ret_message) {
	for (client_const_point_it i = _clients.begin(); i < _clients.end(); ++i) {
		if (&(*it) == *i && ret_message == false) 
			continue;
		send((*i)->get_socket(), (const void *)(str.c_str()), str.length(), 0);
	}
}

void Channel::put_in_topic(std::string& t, client_const_it it) {
	_topic = t;
}

short Channel::put_in_mode(std::string& m, client_const_it it) {
	std::string flags = "opsitnmlbvk";
	if (&(*it) != _clients[0])
		return 1;
	if (m[0] == '+') {
		for (size_t i = 1; i < m.length(); ++i) {
			if (flags.find(m[i]) == std::string::npos)
				return 3;
			if (_mode.find(m[i]) != std::string::npos)
				return 2;
			_mode.push_back(m[i]);
			send_in_channels(":" + it->str_for_irc() + " MODE " + _name + " +" + m[i] + "\r\n", it, true);
		}
	}
	else if (m[0] == '-') {
		for (size_t i = 1; i < m.length(); ++i) {
			if (flags.find(m[i]) == std::string::npos)
				return 3;
			if (_mode.find(m[i]) == std::string::npos)
				return 2;
			send_in_channels(":" + it->str_for_irc() + " MODE " + _name + " -" + m[i] + "\r\n", it, true);
			_mode.erase(_mode.find(m[i]), 1);
		}
	}
	else
		return 0;
	return 0;
}

short Channel::kick_from_channel(std::string& m, client_const_it it) {
	if (&(*it) != _clients[0])
		return 1;
	for (client_const_point_it i = _clients.begin(); i < _clients.end(); ++i) {
		if ((*i)->get_nick() == m) {
			send_in_channels(":" + it->str_for_irc() + " KICK " + _name + " " + (*i)->get_nick() + " :" + it->get_nick() + "\r\n", it, true);
			_clients.erase(i);
			return 0;
		}
	}
	return 2;
}

bool Channel::find_mode(char c) const {
	if (_mode.find(c) == std::string::npos) {
		return false;
	}
	return true;
}

bool Channel::find_nick_in_channel(const std::string& nick) {
	for (client_const_point_it i = _clients.begin(); i < _clients.end(); ++i) {
		if ((*i)->get_nick() == nick)
			return true;
	}
	return false;
}

bool Channel::check_operator(client_const_it it) {
	return _clients[0] == &(*it);
}

void Channel::put_invite(client_const_it it) {
	_invite.push_back(&(*it));
}

void Channel::erase_invite(client_const_it it) {
	if (_invite.size() == 0)
		return;
	for (client_const_point_it i = _invite.begin(); i < _invite.end(); ++i) {
		if (*i == &(*it)) {
			_invite.erase(i);
			return;
		}
	}
}

bool Channel::find_invite(client_const_it it) {
	if (_invite.size() == 0)
		return false;
	for (client_const_point_it i = _invite.begin(); i < _invite.end(); ++i) {
		if (*i == &(*it)) {
			return true;
		}
	}
	return false;
}

void Channel::fix_addr_offset(Client *delete_addr) {
	for (client_point_it it = _clients.begin(); it < _clients.end(); ++it) {
		if (*it > delete_addr)
			--(*it);
	}
	for (client_point_it it = _invite.begin(); it < _invite.end(); ++it) {
		if (*it > delete_addr)
			--(*it);
	}
}

const std::string Channel::get_names_users() const {
	std::string rezult = ":@";
	for (client_const_point_it i = _clients.begin(); i < _clients.end(); ++i) {
		rezult += (*i)->get_nick() + " ";
	}
	return rezult;
}

const std::string& Channel::get_name_channel() const {
	return _name;
}

const std::string& Channel::get_topic() const {
	return _topic;
}

const std::string Channel::get_topic_message(client_const_it it) const {
	if (_topic.size() == 0)
		return " 331 " + it->get_nick() + " " + _name + RPL_NOTOPIC;
	return " 332 " + it->get_nick() + " " + _name + " :" + _topic + "\r\n";
}

const std::string Channel::get_names_message(client_const_it it) const {
	return " 353 " + it->get_nick() + " = " + _name + " " + get_names_users() + "\r\n";
}

const int Channel::get_count_users() const {
	return _clients.size();
}
