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
#include "wrapper/s_chat.h"
#include "wrapper/s_conf.h"
#include "wrapper/s_html.h"
#include "wrapper/s_mutx.h"
#include "wrapper/s_modl.h"
#include "wrapper/s_sock.h"
#include "wrapper/s_lang.h"
#include "wrapper/s_ncur.h"
#include "wrapper/s_sman.h"
#include "wrapper/s_mman.h"
#include "wrapper/s_timr.h"

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
    s_mutx::init(); // init the mutex manager.
    s_conf::init(); // init the config manager.
    s_html::init(); // init the html-template manager.
    s_lang::init(); // init the language manager
    s_sman::init(); // init the session manager.
    s_mman::init(); // init the mysql connection manager.
    s_sock::init(); // init the socket manager.
    s_chat::init(); // init the chat manager.
    s_timr::init(); // init the system timer.

    // begin to draw the ncurses amdin interface in a new pthread.
#ifdef NCURSES

    s_ncur::init(); // init the ncurses admin interface.

    pthread_t admin_thread;
    pthread_create( &admin_thread,
                    NULL,
                    s_ncur::get().start, (void*) &s_ncur::get() );

    // wait until ncurses interface has been initialized.
     while ( ! s_ncur::get().is_ready() )
      usleep(100);
#endif

    pthread_t timer_thread;
    pthread_create( &timer_thread,
                    NULL,
                    s_timr::get().start, (void*) &s_timr::get() );


    s_modl::init(); // init the module-loader manager.

    // start the socket manager. this one will listen for incoming http requests and will
    // forward them to the specified routines which will generate a http response.
    s_sock::get().start();
        

#ifdef VERBOSE
    cout << DOWNMSG << endl;
#endif

    return 0;
}
