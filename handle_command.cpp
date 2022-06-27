#include "includes/handle_command.hpp"
#include "includes/message.hpp"

Handle_command::Handle_command(std::vector<Client>::iterator &i, std::string comm, Server* s) : _server(s), _it(i) {
	char rem = ' ';
	if (comm.front() == ':') {
		_prefix = comm.substr(1, comm.find(' ', 1) - 1);
		comm.erase(0, comm.find(' ', 1));
	}
	if (comm.front() == ' ')
		comm.erase(0, comm.find_first_not_of(' '));
	if (comm.find(' ') > comm.find(':') && comm.find(':') != std::string::npos)
		rem = ':';
	_command = comm.substr(0, comm.find(rem, 1));
	comm.erase(0, _command.length());
	comm.erase(0, comm.find_first_not_of(' '));
	while (comm.size() > 0) {
		comm.erase(0, comm.find_first_not_of(' '));
		rem = ' ';
		if (comm.find(' ') > comm.find(':') && comm.find(':') != std::string::npos)
			rem = ':';
		if (comm[0] == ':') {
			comm.erase(0, comm.find_first_not_of(' ', 1));
			_parametrs.push_back(comm.substr(0, comm.length()));
			break;
		}
		std::string tmp = comm.substr(0, comm.find(rem));
		_parametrs.push_back(tmp);
		comm.erase(0, tmp.length());
	}
	if (_parametrs.size() != 0 && _parametrs.back().length() == 0) {
		_parametrs.pop_back();
	}
	std::cout << "=============================================\n";
	std::cout << "Prefix: \"" << _prefix << "\"" << std::endl;
	std::cout << "Command: \"" << _command << "\"" << std::endl;
	std::cout << "Params: ";
	for (size_t i = 0; i < _parametrs.size(); ++i)
		std::cout << "\"" << _parametrs[i] << "\" ";
	std::cout << std::endl;
	std::cout << "=============================================\n";
}

void Handle_command::handle_exec() {
	if (_it->login() == 0) {
		std::string tmp = do_for_login();
		int count_bytes = send(_it->get_socket(), (const void *)(tmp.c_str()), tmp.length(), 0);
		if (count_bytes == -1) {
			exit(7);
			return;
		}
		if (tmp == "Password incorrect\n") {
			_server->exit_client(_it);
		}
	}
	else
		execute();
}

std::string Handle_command::do_for_login() {
	if (_command == "NICK") {
		if (_parametrs.size() < 1 || _parametrs.size() > 2)
			return put_in_answer(ERR_NEEDMOREPARAMS);
		if (_server->find_nick(_parametrs[0]))
			return put_in_answer(" 433  " + _parametrs[0] + ERR_NICKNAMEINUSE);
		if (_it->get_real_name().size() != 0 && !_it->get_password())
			return put_in_answer("");
		_it->put_nick(_parametrs[0]);
		return (welcome());		
	}
	else if (_command == "USER") {
		if (_parametrs.size() < 4)
			return put_in_answer(ERR_NEEDMOREPARAMS);
		if (_it->get_nick().size() != 0 && !_it->get_password())
			return put_in_answer("");
		_it->put_user_name(_parametrs[0]);
		_it->put_host_name(_parametrs[1]);
		_it->put_server_name(_parametrs[2]);
		_it->put_real_name(_parametrs[3]);
		return (welcome());
	}
	else if (_command == "PASS") {
		if (_parametrs.size() != 1)
			return put_in_answer(ERR_NEEDMOREPARAMS);
		if (_parametrs[0] != _server->get_password())
			return put_in_answer(ERR_PASSWDMISMATCH);
		_it->put_password();
		return (welcome());
		
	}
	return put_in_answer(ERR_NOTREGISTERED);
}

std::string Handle_command::create_welc_message() {
	std::string rezult = ":" + _server->get_name_server() + " 001 ";
	rezult += _it->get_nick() + WELCOME;
	rezult += _it->str_for_irc() + "\r\n";
	return rezult;
}

void Handle_command::send_motd() {
	sendd(_it->get_socket(), put_in_answer(" 375  : -" + _server->get_name_server() + RPL_MOTDSTART));
	sendd(_it->get_socket(), put_in_answer(RPL_MOTD1));
	sendd(_it->get_socket(), put_in_answer(RPL_MOTD2));
	sendd(_it->get_socket(), put_in_answer(RPL_MOTD3));
	sendd(_it->get_socket(), put_in_answer(RPL_MOTD4));
	sendd(_it->get_socket(), put_in_answer(RPL_MOTD5));
	sendd(_it->get_socket(), put_in_answer(RPL_MOTD6));
	sendd(_it->get_socket(), put_in_answer(RPL_ENDOFMOTD));		
}

std::string Handle_command::welcome() {
	if (_it->get_nick().size() != 0 && _it->get_password() && _it->get_real_name().size() != 0) {
		_it->log_in();
		sendd(_it->get_socket(), create_welc_message());
		send_motd();
	}
	return ("");
}

std::string Handle_command::put_in_answer(std::string message) {
	if (message.size() == 0)
		return "Password incorrect\n";
	std::string answer = ":";
	answer += _server->get_name_server();
	answer += message;
	return answer;
}

void Handle_command::execute() {
	if (_command == "QUIT")
		quit();
	else if (_command == "NOTICE")
		privmsg(0);
	else if (_command == "PRIVMSG")
		privmsg(1);
	else if (_command == "OPER")
		oper();
	else if (_command == "KILL")
		kill();
	else if (_command == "RESTART")
		restart();
	else if (_command == "JOIN")
		join();
	else if (_command == "MODE")
		mode();
	else if (_command == "TOPIC")
		topic();
	else if (_command == "INVITE")
		invite();
	else if (_command == "KICK") 
		kick();
	else if (_command == "WHO")
		who();
	else if (_command == "LIST")
		list();
	else if (_command == "PING") 
		ping();
	else if (_command == "PART")
		part();
	else {
		sendd(_it->get_socket(), put_in_answer(" 421 " + _command + ERR_UNKNOWNCOMMAND));
	
	}
}

void Handle_command::sendd(int sock, std::string str) {
	int count_bytes = send(sock, str.c_str(), str.length(), 0);
	if (count_bytes == -1)
		exit(7);
}

std::string Handle_command::create_priv_message(std::string& name, std::string& mes, bool flag) {
	std::string rezult = ":" + _it->str_for_irc();
	if (flag == 0)
		rezult += " NOTICE ";
	else
		rezult += " PRIVMSG ";
	rezult += name + " :" + mes + "\r\n";
	return rezult;
}

void Handle_command::quit() {
	_server->exit_client(_it);
}

void Handle_command::privmsg(bool flag) {
	if (_parametrs.size() != 2) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	std::string one = _parametrs[0];
	while (one.size()) {
		std::string tmp = one.substr(0, one.find(','));
		if (_server->find_nick(tmp)) {
			std::string answer = create_priv_message(tmp, _parametrs[_parametrs.size() - 1], flag);
			sendd(_server->get_socket_client(tmp), answer);
		}
		else if (_server->find_chan(tmp)) {
			std::string answer = create_priv_message(tmp, _parametrs[_parametrs.size() - 1], flag);
			_server->get_chanel(tmp)->send_in_channels(answer, _it, false);
		}
		else {
			sendd(_it->get_socket(), put_in_answer(" 401 " + tmp + ERR_NOSUCHNICK));
			return;
		}
		one.erase(0, tmp.length());
		one.erase(0, one.find_first_not_of(','));
	}
}

void Handle_command::oper() {
	if (_parametrs.size() != 2) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (!_server->find_nick(_parametrs[0])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHNICK));
		return;
	}
	if (_parametrs[1] != _server->get_password() + "_oper") {
		sendd(_it->get_socket(), put_in_answer(ERR_PASSWDMISMATCH));
		return;
	}
	sendd(_it->get_socket(), put_in_answer(RPL_YOUREOPER));
	_it->now_operator();
}

void Handle_command::kill() {
	if (_parametrs.size() > 2 || _parametrs.size() < 1) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (!_server->find_nick(_parametrs[0])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHNICK));
		return;
	}
	if (!_it->get_operator()) {
		sendd(_it->get_socket(), put_in_answer(" 482 " + _parametrs[0] + ERR_NOPRIVILEGES));
		return;
	}
	if (_parametrs.size() == 2)
		sendd(_server->get_socket_client(_parametrs[0]), _parametrs[1] + "\r\n");
	sendd(_server->get_socket_client(_parametrs[0]), ERR_YOUREBANNEDCREEP);
	_server->exit_client(_server->get_client(_parametrs[0]));
}

void Handle_command::restart() {
	if (!_it->get_operator()) {
		sendd(_it->get_socket(), put_in_answer(" 482 " + _parametrs[0] + ERR_NOPRIVILEGES));
		return;
	}
	_server->restart_server();
}

void Handle_command::join() {
	if (_parametrs.size() != 1) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	std::string one = _parametrs[0];
	while (one.size()) {
		std::string tmp = one.substr(0, one.find(','));
		if (tmp.front() != '#') {
			sendd(_it->get_socket(), put_in_answer(" 401 " + tmp + ERR_NOSUCHCHANNEL));
			return;
		}
		if (!_server->find_chan(tmp))
			create_channels(tmp);
		else {
			join_in_channels(tmp);
		}
		one.erase(0, tmp.length());
		one.erase(0, one.find_first_not_of(','));
	}	

}

void Handle_command::part() {
	if (_parametrs.size() != 1) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return ;
	}
	std::string one = _parametrs[0];
	while (one.size()) {
		std::string tmp = one.substr(0, one.find(','));
		if (tmp.front() != '#') {
			sendd(_it->get_socket(), put_in_answer(" 401 " + tmp + ERR_NOSUCHCHANNEL));
			one.erase(0, tmp.length());
			one.erase(0, one.find_first_not_of(','));
			continue;
		}
		if (!_server->find_chan(tmp)) {
			sendd(_it->get_socket(), put_in_answer(" 401 " + tmp + ERR_NOSUCHCHANNEL));
			one.erase(0, tmp.length());
			one.erase(0, one.find_first_not_of(','));
			continue;
		}
		else {
			if (_server->get_chanel(tmp)->get_names_users().find(_it->get_nick()) == std::string::npos)
			{
				sendd(_it->get_socket(), put_in_answer(" 442 " + tmp + ERR_NOTONCHANNEL));
				one.erase(0, tmp.length());
				one.erase(0, one.find_first_not_of(','));
				continue;
			}
			else {
				_server->get_chanel(tmp)->remove_from_channel(_it->get_nick());
				std::string names = (_server->get_chanel(_parametrs[0]))->get_names_users();
				sendd(_it->get_socket(), ":" + _it->str_for_irc() + " PART " + ":" + tmp + "\r\n");
				_server->get_chanel(tmp)->send_in_channels(":" + _it->str_for_irc() + " PART " + ":" + tmp + "\r\n", _it, false);
				if (_server->get_chanel(tmp)->get_count_users() == 0)
					_server->remove_channel(_server->get_chanel(tmp));
				one.erase(0, tmp.length());
				one.erase(0, one.find_first_not_of(','));
			}
		}

	}

}

void Handle_command::send_topic(std::string& tmp) {
	sendd(_it->get_socket(), put_in_answer(_server->get_chanel(tmp)->get_topic_message(_it)));
}

void Handle_command::send_channel_list(std::string& tmp) {
	sendd(_it->get_socket(), put_in_answer(_server->get_chanel(tmp)->get_names_message(_it)));
	sendd(_it->get_socket(), put_in_answer(" 366 " + _it->get_nick() + " " + tmp + RPL_ENDOFNAMES));
}

void Handle_command::create_channels(std::string& tmp) {
	_server->create_channels(tmp, _it);
	std::string names = (_server->get_chanel(tmp))->get_names_users();
	sendd(_it->get_socket(), ":" + _it->str_for_irc() + " JOIN " + ":" + tmp + "\r\n");
	send_topic(tmp);
}

void Handle_command::join_in_channels(std::string& tmp) {
	if (_server->get_chanel(tmp)->find_nick_in_channel(_it->get_nick())) {
		sendd(_it->get_socket(), put_in_answer(" 443 " + _it->get_nick() + " " + tmp + ERR_USERONCHANNEL));
		return;		
	}
	if (_server->get_chanel(tmp)->find_mode('i')) {
		if (!_server->get_chanel(tmp)->find_invite(_it)) {
			sendd(_it->get_socket(), put_in_answer(" 404 " + tmp + ERR_NOSUCHCHANNEL));
			return;
		}
	}
	_server->get_chanel(tmp)->erase_invite(_it);
	_server->add_in_channel(tmp, _it);
	std::string names = (_server->get_chanel(tmp))->get_names_users();
	_server->get_chanel(tmp)->send_in_channels(":" + _it->str_for_irc() + " JOIN " + ":" + tmp + "\r\n", _it, true);
	send_topic(tmp);
}

void Handle_command::mode() {
	if (_parametrs.size() < 1 || _parametrs.size() > 2) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (!_server->find_chan(_parametrs[0])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
		return;
	}
	if (_parametrs.size() == 2) {
		short status = _server->get_chanel(_parametrs[0])->put_in_mode(_parametrs[1], _it);
		switch (status)
		{
		case 1:
			sendd(_it->get_socket(), put_in_answer(" 482 " + _parametrs[0] + ERR_CHANOPRIVSNEEDED));
			break;
		case 2:
			sendd(_it->get_socket(), put_in_answer(" 467 " + _parametrs[0] + ERR_KEYSET));
			break;
		case 3:
			sendd(_it->get_socket(), put_in_answer(ERR_UMODEUNKNOWNFLAG));
			break;		
		}
	}
}

void Handle_command::topic() {
	if (_parametrs.size() != 2) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (!_server->find_chan(_parametrs[0])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
		return;
	}
	if (_server->get_chanel(_parametrs[0])->find_mode('t')) {
		if (!_server->get_chanel(_parametrs[0])->check_operator(_it)) {
			sendd(_it->get_socket(), put_in_answer(" 482 " + _parametrs[0] + ERR_CHANOPRIVSNEEDED));
			return;		
		}
		_server->get_chanel(_parametrs[0])->put_in_topic(_parametrs[1], _it);
		_server->get_chanel(_parametrs[0])->send_in_channels(":" + _it->str_for_irc() + " TOPIC " + _parametrs[0] + " :" + _parametrs[1] + "\r\n", _it, true);
	}
	else {
		if (!_server->get_chanel(_parametrs[0])->find_nick_in_channel(_it->get_nick())) {
			sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
			return;		
		}
		_server->get_chanel(_parametrs[0])->put_in_topic(_parametrs[1], _it);
		_server->get_chanel(_parametrs[0])->send_in_channels(":" + _it->str_for_irc() + " TOPIC " + _parametrs[0] + " :" + _parametrs[1] + "\r\n", _it, true);
	}
}

void Handle_command::invite() {
	if (_parametrs.size() != 2) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (!_server->find_nick(_parametrs[0])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHNICK));
		return;
	}
	if (!_server->find_chan(_parametrs[1])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
		return;
	}
	if (!_server->get_chanel(_parametrs[1])->check_operator(_it)) {
		sendd(_it->get_socket(), put_in_answer(" 482 " + _parametrs[0] + ERR_CHANOPRIVSNEEDED));
		return;		
	}
	_server->get_chanel(_parametrs[1])->put_invite(_server->get_client(_parametrs[0]));
	sendd(_server->get_socket_client(_parametrs[0]), put_in_answer(" INVITE " + _parametrs[0] + " :" + _parametrs[1] + "\r\n"));
}

void Handle_command::kick() {
	if (_parametrs.size() != 2) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (!_server->find_chan(_parametrs[0])) {
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
		return;
	}
	short status = _server->get_chanel(_parametrs[0])->kick_from_channel(_parametrs[1], _it);
	switch (status)
	{
	case 1:
		sendd(_it->get_socket(), put_in_answer(" 482 " + _parametrs[0] + ERR_CHANOPRIVSNEEDED));
		break;
	case 2:
		sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
		break;		
	}
}

void Handle_command::who() {
	if (_parametrs.size() != 1) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	if (_parametrs[0][0] == '#') {
		if (!_server->find_chan(_parametrs[0])) {
			sendd(_it->get_socket(), put_in_answer(" 401 " + _parametrs[0] + ERR_NOSUCHCHANNEL));
			return;
		}
		send_channel_list(_parametrs[0]);
	}
	else if (_parametrs[0][0] == '*' && _parametrs[0].length() == 1) {
		for (size_t i = 1; i < _server->count_clients(); ++i) {
			Client& tmp = _server->return_client(i);
			sendd(_it->get_socket(), put_in_answer(" 352 " + _it->get_nick() + " * "  + tmp.get_user_name() + " " + tmp.get_server_name() + " " + _server->get_name_server() + " " + tmp.get_nick() + " H :0 " + tmp.get_real_name() + "\r\n"));
		}
		sendd(_it->get_socket(), put_in_answer(" 315 " + _it->get_nick() + " " + _it->get_nick() + RPL_ENDOFWHO));
	}
	else {
		if (_server->find_nick(_parametrs[0])) {
			Client& tmp  = *(_server->get_client(_parametrs[0]));
			sendd(_it->get_socket(), put_in_answer(" 352 " + _it->get_nick() + " * "  + tmp.get_user_name() + " " + tmp.get_server_name() + " " + _server->get_name_server() + " " + tmp.get_nick() + " H :0 " + tmp.get_real_name() + "\r\n"));
			sendd(_it->get_socket(), put_in_answer(" 315 " + _it->get_nick() + " " + _it->get_nick() + RPL_ENDOFWHO));
		}
	}
}

void Handle_command::list(void) {
	if (_parametrs.size() != 0) {
		sendd(_it->get_socket(), put_in_answer(ERR_NEEDMOREPARAMS));
		return;
	}
	for (size_t i = 0; i < _server->count_channels(); ++i) {
		Channel& tmp = _server->return_channel(i);
		sendd(_it->get_socket(), put_in_answer(" 322 " + _it->get_nick() + " " + tmp.get_name_channel() + " " + std::to_string(tmp.get_count_users()) + " :" + tmp.get_topic() + "\r\n"));
	}
	sendd(_it->get_socket(), put_in_answer(" 323 " + _it->get_nick() + " " + RPL_ENDOFLIST));
}

void Handle_command::ping() {
	typedef std::vector<std::string>::iterator iter;
	std::string answer = "PONG ";
	for (iter i = _parametrs.begin(); i < _parametrs.end(); ++i) {
		answer += *i + " ";
	}
	answer.pop_back();
	answer += "\r\n";
	sendd(_it->get_socket(), answer);
}
