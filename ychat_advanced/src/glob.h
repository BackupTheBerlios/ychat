// global variables.

#ifndef GLOB_H
#define GLOB_H

#include <map>
#include <pthread.h>

// definition of boolean values.
#define true   1
#define false  0

// config filename. ( will be searched in ~/.ychat/ and if not found in the working 
// directory of ychat ).
#define CONFILE "conf.txt"

// the hard limit of mysql connections at the samte time in the mysql connection
// manager.
#define MAXMSQL 20

// defines the standard value of the init method of the mysql connection manager
// for the mysql server port. 
#define PRTMSQL 3306

// the highest port which is allowed to use. if ychat is unable to create the server
// socket it will increment the port number and tries to create another socket.
// this procedure will go on until MAXPORT is reached.
#define MAXPORT 65535

// max length of a line read from a socket or a file ( config-file, html-template ).
#define READBUF 1024

// max length of a char buffer
#define CHARBUF 100

// max occupied percentage of a hash map:
#define HMAPOCC 80

// Prints more messages if SERVMSG or NCURSES is defined (such as each http request) 
//#define VERBOSE

// is used if all mysql queries should be printed out.
#define QUERIES

// displays important server messages. this one will print all messages
// in stdout if no NCURSES is defined.  
// dont use this until NCURSES is defined! all messages will appear in the
// ncurses interface anyways.
//#define SERVMSG

// is used for interaction with the ncurses admin interface at stdout.
#define NCURSES


// The following values define the positions of the data stats in the NCURSES interface.
#ifdef NCURSES
#define NCUR_HITS_X 21
#define NCUR_HITS_Y 35
#define NCUR_GARBAGE_X 22
#define NCUR_GARBAGE_Y 35
#define NCUR_MYSQL_X 22
#define NCUR_MYSQL_Y 2
#define NCUR_PORT_X 21
#define NCUR_PORT_Y 2
#define NCUR_SESSION_X 22
#define NCUR_SESSION_Y 18 
#define NCUR_THREADS_X 21
#define NCUR_THREADS_Y 18 
#define NCUR_UPTIME_X 22
#define NCUR_UPTIME_Y 62
#define NCUR_TIME_X 21
#define NCUR_TIME_Y 62
#endif

// Defines the amount of newlines which have to send to the client's
// chat stream the first log-in. ( prevents white screen because of buffers
// or proxies ).
#define PUSHSTR 1000

//////////////////////////////////////////////////////////////////////////////////////////
// DO OFFT CHANGE ANYTHING BEHIND THIS LINE!
//////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

// internal rang descriptors ( their external names may be specified different )
enum rang
{
    OPERATOR , // Operator.
    HALF_OP ,  // Half-Operator.
    VOICE ,    // Normal user with temp. extra priv. 
    OFFRMAL ,   // Normal user. 
    GUEST      // Guest 
};

// some custom typedefs for datatypes which are needed often.
typedef map<string, string> map_string;
typedef int function( void *v_arg );

struct container
{
    void* elem[4];
};

struct dynmod
{
    function *the_func  ;
    void     *the_module;
};

// Define external executables:
#define GMAKE "/usr/local/bin/gmake \0"
#endif
