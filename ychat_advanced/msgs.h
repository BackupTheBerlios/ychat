#ifndef MSGS_H
#define MSGS_H


// several error messages which will apear by the clients.
#define E_ALPNUM "The nick you have specified is not alphanumeric, please change that.<br><br>"
#define E_NONICK "You need to specify a nick name.<br><br>"
#define E_NOTONL "An error occured. Your nick is not online.<br><br>"
#define E_ONLINE "The nick you have specified is already online. Try another nick.<br><br>"

// alphabetical ordered:
#define ACCPERR "Sock accept error "
#define BINDERR "Sock bind error "
#define CFILEOK "Parsing config file "
#define CFILENO "Failed opening config file "
#define CONTACT "Contact: www.yChat.org, Mail@yChat.org, ICQ: 11655527 "
#define DESCRIP "yChat++ Copyright (C) 2003 Paul Buetow, Volker Richter " 
#define DOWNMSG "Shutting down "
#define LISTERR "Sock listen error "
#define MODULEC "Caching module "
#define NEWROOM "New room "
#define NEWUSER "New user "
#define READERR "Sock read error "
#define REMUSER "Removing user "
#define REQUEST "Request string "
#define SEPERAT "-----------------------"
#define SELCERR "Sock select error "
#define SOCKCRT "Creating server socket\0"
#define SOCKERR "Couldn't create socket "
#define SOCKRDY "Server socket is ready "
#define STARTMS "Starting up "
#define TECACHE "Caching template "
#define VERSION "Version: CVS " 

#endif
