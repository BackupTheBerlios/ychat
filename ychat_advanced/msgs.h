#ifndef MSGS_H
#define MSGS_H


// several error messages which will apear by the clients.
#define E_ALPNUM "The nick you have specified is not alphanumeric, please change that.<br><br>\0"
#define E_NONICK "You need to specify a nick name.<br><br>\0"
#define E_NOTONL "An error occured. Your nick is not online.<br><br>\0"
#define E_ONLINE "The nick you have specified is already online. Try another nick.<br><br>\0"

// alphabetical ordered:
#define ACCPERR "Sock accept error \0"
#define BINDERR "Sock bind error \0"
#define CFILEOK "Parsing config file \0"
#define CFILENO "Failed opening config file \0"
#define CONTACT "Contact: http://www.yChat.org, Mail@yChat.org, ICQ: 11655527\0"
#define DESCRIP "yChat++: Copyright (C) 2003 Paul Buetow, Volker Richter\0" 
#define DOWNMSG "Shutting down \0"
#define LISTERR "Sock listen error \0"
#define MODULEC "Caching module \0"
#define NEWREQU "New request \0"
#define NEWROOM "New room \0"
#define NEWUSER "New user \0"
#define READERR "Sock read error \0"
#define REMUSER "Removing user \0"
#define REQUEST "Request string \0"
#define SEPERAT "-----------------------\0"
#define SELCERR "Sock select error \0"
#define SOCKCRT "Creating server socket \0"
#define SOCKERR "Couldn't create socket \0"
#define SOCKRDY "Server socket is ready \0"
#define STARTMS "Starting up \0"
#define TECACHE "Caching template \0"
#define VERSION "Version: 0.4 CVS \0" 

#endif
