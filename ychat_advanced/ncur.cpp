#ifndef NCUR_CPP
#define NCUR_CPP

#include "ncur.h"

using namespace std;

ncur::ncur( )
{
    p_messagelist = new list<char*>;
    pthread_mutex_init( &mut_messages, NULL );
    i_message_length = 46;
    b_is_ready = false;
}

ncur::~ncur()
{
    pthread_mutex_destroy( &mut_messages );
}

void*
ncur::start( void *v_pointer )
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    ncur* admin_interface = static_cast<ncur*>(v_pointer);

    initscr();
    start_color();
    clear();
    noecho();
    cbreak();       // Line buffering disabled. pass on everything

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    mvprintw( 0,2, VERSION);

    refresh();

    char *choices[] = {
                          "Unload all modules      ",
                          "Reload all modules      ",
                          "Clear template cache    ",
                          "Run garbage collector   ",
                          "Module-loader manager NI",
                          "MySQL manag.          NI",
                          "Session manager       NI",
                          "Socket manager        NI",
                          "Shut down server",
                      };

    admin_interface->p_serveroutput = newwin( 19, 49, 1, 31 );
    wbkgd(admin_interface->p_serveroutput, COLOR_PAIR(1));

    box      ( admin_interface->p_serveroutput, 0, 0 );
    mvwprintw( admin_interface->p_serveroutput, 2, 2, "SERVER SYSTEM MESSAGES" );
    wrefresh ( admin_interface->p_serveroutput );


    admin_interface->print( VERSION );
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );

#ifdef NCURSES
    wrap::SMAN->print_init_ncurses(); 
#endif

    admin_interface->is_ready( true );
    admin_interface->p_menu = new menu( 1, 1, 30, 19, "ADMINISTRATOR's MAIN MENU", choices, 9, COLOR_PAIR(1));
    admin_interface->p_menu->start( &switch_main_menu_ );

    admin_interface->shutdown();	
}

void
ncur::shutdown()
{
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
    {
	string s_tmp( c_print );
	print( s_tmp.substr( 0, i_message_length ) );
        print( s_tmp.substr( i_message_length, s_tmp.length() ) ); 
	return;
    }	

    char* c_temp = new char[i_message_length];
    memcpy( c_temp, c_print, strlen(c_print) );
    int i;
    for ( i = strlen(c_print); i < i_message_length; i++ )
        c_temp[i] = ' ';
    c_temp[i] = '\0';

    pthread_mutex_lock( &mut_messages );

    if ( p_messagelist->size() > 12 )
    {
        char* c_front = p_messagelist->front();
        p_messagelist->pop_front();
        free( c_front );
    }

    p_messagelist->push_back( c_temp );

    list<char*>::iterator iter;
    iter = p_messagelist->begin();

    for ( i=4; i<18 && iter != p_messagelist->end(); i++, iter++ )
        mvwprintw( p_serveroutput, i, 2, *iter );

    wrefresh ( p_serveroutput );

    pthread_mutex_unlock( &mut_messages );
}

void
ncur::switch_main_menu_( int i_choice )
{
    if( i_choice != 0 )
        switch ( i_choice )
        {
        case 1:
            wrap::MODL->unload_modules();
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Unloaded all modules               ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 2:
            wrap::MODL->reload_modules();
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Reloaded all modules               ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 3:
            wrap::HTML->clear_cache();
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Cleared the template cache         ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 4:
            if ( ! wrap::GCOL->remove_garbage() )
             wrap::NCUR->print( GARNONE );
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Garbage collector activated        ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 9: // Shut down server
	    wrap::MMAN->~mman();
            mvprintw( 21,2, "Good bye !");
            refresh();
            clrtoeol();
            refresh();
            endwin();
            exit(0);
            break;

        default:
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Selection # %d not yet implemented!", i_choice-1);
            wrap::NCUR->print( "Selection not yet implemented!" );
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        }
}
#endif
