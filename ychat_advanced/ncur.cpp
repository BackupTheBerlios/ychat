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

void
ncur::start()
{

 initscr();
 clear();
 noecho();
 cbreak();       // Line buffering disabled. pass on everything 

 mvprintw(0, 0, DESCRIP );
 mvprintw(1, 0, CONTACT );
 refresh();

 char *choices[] = { 
  "Chat manager NI",
  "Config manager NI",
  "HTML-template manager NI",
  "Language manager NI",
  "Module-loader manager NI",
  "MySQL connecion manager NI",
  "Session manager NI",
  "Socket manager NI",
  "Exit NI",
 };

 this->p_menu = new menu( 2, 4, 34, 15, "MAIN MENU", choices, 9 );
 p_menu->start();

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
