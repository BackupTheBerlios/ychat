#ifndef SOCK_CPP
#define SOCK_CPP

#include <unistd.h>
#include "sock.h"
#include "chat.h"
#include "user.h"

using namespace std;

sock::sock()
{
    this->b_run      = true;
    this->i_req      = 0;
    this->i_threads  = 2; // currently there are 2 threads running ( system timer and  server listening );
#ifdef NCURSES
    // we need to add the ncurses admin interface thread!
    this->i_threads++;
#endif
    this->req_parser = new reqp();
    this->thrd_pool  = new pool();
    this->log_daemon = new logd( wrap::CONF->get_elem( "ACCESS_LOG" ));

    pthread_mutex_init( &mut_threads, NULL );
}

sock::~sock()
{
    pthread_mutex_destroy( &mut_threads );
}

void
sock::chat_stream( int i_sock, user* p_user, map_string &map_params )
{
    string s_msg( "\n" );

    for ( int i = 0; i < PUSHSTR; i++ )
        send( i_sock, s_msg.c_str(), s_msg.size(), 0 );

    do
    {
        pthread_mutex_lock  ( &(p_user->mut_message) );
        s_msg = p_user->get_mess( );
        if ( 0 > send( i_sock, s_msg.c_str(), s_msg.size(), 0 ) )
            p_user->set_online( false );
        pthread_cond_wait( &(p_user->cond_message), &(p_user->mut_message) );
    }
    while( p_user->get_online() );

    // if there is still a message to send:
    s_msg = p_user->get_mess( );
    if ( ! s_msg.empty() )
        send( i_sock, s_msg.c_str(), s_msg.size(), 0 );

    pthread_mutex_unlock( &(p_user->mut_message) );

    // remove the user from its room.
    string s_user( p_user->get_name() );
    p_user->get_room()->del_elem( s_user );

    // post the room that the user has left the chat.
    s_msg = wrap::TIMR->get_time() + " " 
          + p_user->get_colored_bold_name()
          + wrap::LANG->get_elem( "USERLEAV" )
          + "<br>\n";

    p_user->get_room()->msg_post( &s_msg );


#ifdef VERBOSE
    cout << REMUSER << s_user << endl;
#endif

    wrap::GCOL->add_user_to_garbage( p_user );
}

int
sock::make_socket( uint16_t i_port )
{
    int sock;
    struct sockaddr_in name;

    // create the server socket.
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        wrap::system_message( SOCKERR );

        if ( ++i_port > MAXPORT )
            exit(-1);

        wrap::system_message( SOCKERR );

        return make_socket( i_port );
    }

    // give the server socket a name.
    name.sin_family = AF_INET;
    name.sin_port = htons (i_port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    int optval=1;

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));

    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {

     wrap::system_message( BINDERR );

        if ( ++i_port > MAXPORT )
            exit(-1);

        wrap::system_message( SOCKERR + i_port );

        return make_socket( i_port );
    }

    wrap::system_message( SOCKCRT + string("localhost:") + tool::int2string(i_port) );

#ifdef NCURSES
    mvprintw( NCUR_PORT_X,NCUR_PORT_Y, "Port: %d ", i_port);
    refresh();
#endif

    return sock;
}

int
sock::read_write( thrd* p_thrd, int i_sock )
{
    char c_req[2048];

    int i_bytes;
    i_bytes = read (i_sock, c_req, 2048);

    if (i_bytes < 0)
    {
     wrap::system_message( READERR );
    }
    else
    {
        // stores the request params.
        map_string map_params;

        // get the s_rep ( s_html response which will be send imediatly to the client
        // and fill map_params with request values.
        auto string s_temp=(string)c_req;
        struct sockaddr_in client;
        size_t size=sizeof(client);

        getpeername( i_sock, (struct sockaddr *)&client, &size);

        map_params["REMOTE_ADDR"]=inet_ntoa(client.sin_addr);
        map_params["REMOTE_PORT"]=ntohs( client.sin_port);


        string s_rep = req_parser->parse( p_thrd, string( c_req ), map_params );
        // send s_rep to the client.
        log_daemon->log(map_params);

        send( i_sock, s_rep.c_str(), s_rep.size(), 0 );

        // dont need those vals anymore.
        map_params.clear();

        return 0;
    }

    return -1;
}

int
sock::start()
{
    wrap::system_message( STARTMS );
#ifdef NCURSES
    print_hits();
    print_threads();
#endif

    auto int i_port = tool::string2int( wrap::CONF->get_elem( "SRVRPORT" ) );

    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    size_t size;


    // create the server socket and set it up to accept connections.
    sock = make_socket ( i_port );

    if (listen (sock, 1) < 0)
    {
      wrap::system_message( LISTERR );
      exit( EXIT_FAILURE );
    }

    wrap::system_message( SOCKRDY );

    // initialize the set of active sockets.
    FD_ZERO (&active_fd_set);
    FD_SET  (sock, &active_fd_set);

    while( b_run )
    {
        // block until input arrives on one or more active sockets.
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
         wrap::system_message( SELCERR );

         exit( EXIT_FAILURE );
        }

        // service all the sockets with input pending.
        for ( i = 0; i < FD_SETSIZE; i++ )
            if ( FD_ISSET (i, &read_fd_set) )
            {
                if ( i == sock )
                {
                    // connection request on original socket.
                    i_req++;
#ifdef NCURSES
                    print_hits();
#endif

                    int new_sock;
                    size = sizeof (clientname);
                    new_sock = accept (sock,
                                       (struct sockaddr *) &clientname,
                                       &size);

                    if (new_sock < 0)
                    {
	             wrap::system_message( ACCPERR );
                     close ( new_sock );
                    }
		
                    else	
		    {
#ifdef VERBOSE
    		     pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
                     cout << NEWREQU << i_req << " "
                     << inet_ntoa( clientname.sin_addr )
                     << ":"
                     << ntohs    ( clientname.sin_port )
                     << endl;
    		     pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
                     FD_SET (new_sock, &active_fd_set);
                    }
                }

                else
                {

                    thrd_pool->run( (void*) new thrd( i ) );
                    FD_CLR( i, &active_fd_set );
                }
            }
    }
}

void
sock::increase_num_threads()
{
    pthread_mutex_lock( &mut_threads );
    i_threads++;
    pthread_mutex_unlock( &mut_threads );
#ifdef NCURSES
    print_threads();
#endif

}

void
sock::decrease_num_threads()
{
    pthread_mutex_lock( &mut_threads );
    i_threads--;
    pthread_mutex_unlock( &mut_threads );

#ifdef NCURSES
    print_threads();
#endif

}

#ifdef NCURSES
void
sock::print_threads()
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    mvprintw( NCUR_THREADS_X,NCUR_THREADS_Y, "Threads: %d ", i_threads);
    refresh();
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
}

void
sock::print_hits()
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    mvprintw( NCUR_HITS_X,NCUR_HITS_Y, "Hits: %d ", i_req);
    refresh();
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
}
#endif

#endif
