/*
 * yChat++; Contact: www.yChat.org; Mail@yChat.org
 * Copyright (C) 2003 Paul C. Buetow, Volker Richter 
 * -----------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <unistd.h>
#include <signal.h>
#include "incl.h"

using namespace std;

int main()
{
#ifdef VERBOSE
    cout  <<  "         ___ _           _   "     << endl
    <<  " _   _  / __\\ |__   __ _| |_ "    << endl
    <<  "| | | |/ /  | '_ \\ / _` | __|"    << endl
    <<  "| |_| / /___| | | | (_| | |_ "     << endl
    <<  " \\__, \\____/|_| |_|\\__,_|\\__|" << endl
    <<  " |___/ 			"     << endl << endl

    << DESCRIP << endl
    << VERSION << ", "
    << CONTACT << endl
    << SEPERAT << endl
    << STARTMS << endl ;
#endif

    // ignore SIGPIPE. otherwise the server will shut down with "Broken pipe" if
    // a client unexpected disconnects himself from a SOCK_STREAM.
    signal( SIGPIPE, SIG_IGN );

    // all the static data classes have to be initialized once. otherwise they will
    // contain only empty pointers and the chat server won't work correctly.
    // the order of the initializations is very importand. for example the s_html::init()
    // invokations assumes an initialized s_conf class.

    // init the dynamic wrapper (is needed to pass all wrapped objects through a single pointer).
    wrap::WRAP = new dynamic_wrap;

    // init the mutex manager.
    wrap::WRAP->MUTX = wrap::MUTX = new mutx(); 

    // init the config manager.
    wrap::WRAP->CONF = wrap::CONF = new conf( CONFILE ); 

    // init the html-template manager.
    wrap::WRAP->HTML = wrap::HTML = new html(); 

    // init the language manager
    wrap::WRAP->LANG = wrap::LANG = new lang( wrap::CONF->get_val( "LANGUAGE") ); 

    // init the session manager.
    wrap::WRAP->SMAN = wrap::SMAN = new sman(); 

    // init the mysql connection manager.
    wrap::WRAP->MMAN = wrap::MMAN = new mman(); 

    // init the socket manager.
    wrap::WRAP->SOCK = wrap::SOCK = new sock(); 

    // init the chat manager.
    wrap::WRAP->CHAT = wrap::CHAT = new chat(); 

    // init the system timer.
    wrap::WRAP->TIMR = wrap::TIMR = new timr(); 

    // begin to draw the ncurses amdin interface in a new pthread.
#ifdef NCURSES

    wrap::WRAP->NCUR = wrap::NCUR = new ncur(); // init the ncurses admin interface.

    pthread_t admin_thread;
    pthread_create( &admin_thread,
                    NULL,
                    wrap::NCUR->start, (void*) wrap::NCUR );

    // wait until ncurses interface has been initialized.
     while ( ! wrap::NCUR->is_ready() )
      usleep(100);
#endif

    pthread_t timer_thread;
    pthread_create( &timer_thread,
                    NULL,
                    wrap::TIMR->start, (void*) wrap::TIMR );


    wrap::WRAP->MODL = wrap::MODL = new modl(); // init the module-loader manager.

    // start the socket manager. this one will listen for incoming http requests and will
    // forward them to the specified routines which will generate a http response.
    wrap::SOCK->start();
        

#ifdef VERBOSE
    cout << DOWNMSG << endl;
#endif

    return 0;
}
