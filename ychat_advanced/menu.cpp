#ifndef MENU_CXX
#define MENU_CXX

#include "menu.h"
#include "s_sock.h"

using namespace std;

menu::menu( int i_startx, int i_starty, int i_width, int i_height, char *c_header, char **choices, int i_numchoices )	
{
 this->i_startx = i_startx;
 this->i_starty = i_starty;
 this->i_height = i_height;
 this->i_width  = i_width;
 this->c_header = c_header;
 this->choices  = choices;
 this->i_numchoices = i_numchoices; 

 initialize();
}

menu::~menu()
{
/*
 wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
 wrefresh(win);
 delwin(win);
*/
}

void
menu::initialize()
{
 this->i_highlight = 1;
 this->i_choice = 0;

 win = newwin( i_height, i_width, i_starty, i_startx);
 keypad(win, TRUE);
}

void
menu::display()
{
 int x, y, i;    

 x = 2;
 y = 2;

 box( win, 0, 0 );
 mvwprintw( win, y++, x, "%s", c_header );

 for( i=0; i<i_numchoices; i++ )
 {       
  y++;

  if( i_highlight == i + 1 ) /* High light the present choice */
  {       
   wattron( win, A_REVERSE); 
   mvwprintw( win, y, x, "%d. %s", i+1, choices[i]);
   wattroff( win, A_REVERSE);
  }

  else
  {
   mvwprintw( win, y, x, "%d. %s", i+1, choices[i]);
  }
 }

 wrefresh( win );
}

void
menu::start()
{

 refresh();
 bool b_flag = 1;

 while( b_flag )
 {  
  display();
  c = wgetch( win );

  switch(c)
  {       
   case KEY_UP:
    if( i_highlight == 1 )
     i_highlight = i_numchoices;
    else
     --i_highlight;
   break;

   case KEY_DOWN:
    if( i_highlight == i_numchoices )
     i_highlight = 1;
    else 
     ++i_highlight;
   break;

   case 10: i_choice = i_highlight; break;

   default:
    mvprintw( 0, 3, "%3d %c", c, c);
    refresh();
   break;
  }

  if( i_choice != 0 )
   switch ( i_choice )
   {
    case 9: // Shut down server
     mvprintw( 20,3, "Good bye !");
     refresh();
     clrtoeol();
     refresh();
     endwin();
     exit(0);
    break; 

    default: 
     mvprintw( 20,3, "Selection # %d not yet implemented!", i_choice); 
     refresh(); 
     break;
   }
 }       
}
#endif
