#ifndef NCUR_CXX
#define NCUR_CXX

#include "ncur.h"

using namespace std;

ncur::ncur( )
{
}

ncur::~ncur()
{
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

// admin_interface->p_info = newwin( 34, 15, 15, 36); 

 admin_interface->p_menu = new menu( 2, 4, 32, 15, "ADMIN MAIN MENU", choices, 9 );
 admin_interface->p_menu->start();

 clrtoeol();
 refresh();
 endwin();
}

void
ncur::print( string &s_print )
{
}

void
ncur::print( char* c_print )
{
}

#endif
