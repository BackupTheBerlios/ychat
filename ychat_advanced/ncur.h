#ifndef NCUR_H
#define NCUR_H

#include <ncurses.h> 
#include "incl.h"
#include "menu.h"

using namespace std;

class ncur 
{
private: 
 menu*  p_menu;

public:
 explicit ncur( );      	      // a standard constructor.
         ~ncur( );

 void start();
 void print( string &s_print );
 void print( char* c_print );

};

#endif
