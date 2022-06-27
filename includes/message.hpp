#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "server.hpp"

#define WELCOME " :Welcome to the Internet Relay Network "
#define RPL_MOTDSTART "- Message of the day -\r\n"
#define RPL_MOTD1 " 372  : \r\n"
#define RPL_MOTD2 " 372  : HELLO, WORLD!!!\r\n"
#define RPL_MOTD3 " 372  :    ヽ༼ ಠ益ಠ ༽ﾉ\r\n"
#define RPL_MOTD4 " 372  : WELCOME TO IRC SERVER\r\n"
#define RPL_MOTD5 " 372  : ༼∵༽ ༼⍨༽ ༼⍢༽ ༼⍤༽\r\n"
#define RPL_MOTD6 " 372  :   ༼⍢༽ ༼⍨༽ ༼⍤༽ ༼∵༽\r\n"
#define RPL_ENDOFMOTD " 376  : End of /MOTD command\r\n"
#define RPL_YOUREOPER " 381 :You are now an IRC operator\r\n"
#define ERR_YOUREBANNEDCREEP " 465 :You are banned from this server\r\n"
#define RPL_ENDOFWHO " :End of /WHO list\r\n"
#define RPL_ENDOFLIST " :End of /LIST\r\n"
//CHANNELS
#define RPL_NOTOPIC " :No topic is set\r\n"
#define RPL_ENDOFNAMES " :End of /NAMES list\r\n"
#define ERR_CHANOPRIVSNEEDED " :You're not channel operator\r\n"
#define ERR_KEYSET " :Channel key already set\r\n"
#define ERR_UMODEUNKNOWNFLAG " 501 :Unknown MODE flag\r\n"
#define ERR_USERONCHANNEL " :is already on channel\r\n"
//ERRORS
#define ERR_NOSUCHNICK " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL " :No such channel\r\n"
#define ERR_NICKNAMEINUSE " :Nickname is already in use\r\n"
#define ERR_UNKNOWNCOMMAND " :Unknown command\r\n"
#define ERR_NOTREGISTERED " 451  :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS " 461  OPER :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH " 464  :Password incorrect\r\n"
#define ERR_NOPRIVILEGES " :Permission Denied- You're not an IRC operator\r\n"
#define ERR_NOTONCHANNEL " :You're not on that channel\r\n"

// 442     ERR_NOTONCHANNEL
//                         "<channel> :You're not on that channel"
#endif