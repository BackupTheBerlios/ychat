#ifndef NCUR_CXX
#define NCUR_CXX

#include "ncur.h"
#include "s_sock.h"

using namespace std;

ncur::ncur( )
{
 p_messagelist = new list<char*>;
 pthread_mutex_init( &mut_messages, NULL );
 i_message_length = 41;
 b_is_ready = false; 
}

ncur::~ncur()
{
 pthread_mutex_destroy( &mut_messages );
}

void*
ncur::start( void *v_pointer )
{
 ncur* admin_interface = static_cast<ncur*>(v_pointer);

 initscr();
 clear();
 noecho();
 cbreak();       // Line buffering disabled. pass on everything 

 mvprintw(1, 3, DESCRIP );
 mvprintw(2, 3, CONTACT );
 refresh();

 char *choices[] = { 
  "Chat manager            NI",
  "Config manager          NI",
  "HTML-template manager   NI",
  "Language manager        NI",
  "Module-loader manager   NI",
  "MySQL connecion manager NI",
  "Session manager         NI",
  "Socket manager          NI",
  "Shut down server",
 };

 admin_interface->p_serveroutput = newwin( 15, 45, 4, 34 );

 box      ( admin_interface->p_serveroutput, 0, 0 ); 
 mvwprintw( admin_interface->p_serveroutput, 2, 2, "SERVER SYSTEM MESSAGES" );
 wrefresh ( admin_interface->p_serveroutput ); 

 admin_interface->is_ready( true );

 admin_interface->print( VERSION );

 admin_interface->p_menu = new menu( 2, 4, 32, 15, "ADMIN MAIN MENU", choices, 9 );
 admin_interface->p_menu->start();

 clrtoeol();
 refresh();
 endwin();
}

void
ncur::print( string *p_msg )
{
 print( (char*)p_msg->c_str() );
}

void
ncur::print( string s_msg )
{
 print( (char*)s_msg.c_str() );
}

void
ncur::print( char* c_print )
{
 if ( strlen( c_print ) > i_message_length )
  c_print[i_message_length] = '\0';

 char* c_temp = new char[i_message_length];
 int i;
 for ( i = 0; i < i_message_length-1; i++ )
  c_temp[i] = ' ';
 c_temp[i] = '\0';

 memcpy( c_temp, c_print, strlen(c_print) ); 

 pthread_mutex_lock( &mut_messages );
 if ( p_messagelist->size() > 8 )
   p_messagelist->pop_front();

 p_messagelist->push_back( c_temp );

 list<char*>::iterator iter;
 iter = p_messagelist->begin();

 for ( i=4; i<14 && iter != p_messagelist->end(); i++, iter++ )
 {
  mvwprintw( p_serveroutput, i, 2, *iter );
 }

 wrefresh ( p_serveroutput ); 

 pthread_mutex_unlock( &mut_messages );
}

#endif
