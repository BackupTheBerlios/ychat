#ifndef NCUR_CPP
#define NCUR_CPP

#include "ncur.h"

using namespace std;

const string GMAKE_PARAMS[] = { "clean_base", "clean_modules",  "all" };
const int    GMAKE_ELEMENTS = 3;

ncur::ncur( )
{
    p_messagelist = new list<char*>;
    pthread_mutex_init( &mut_messages, NULL );
    i_message_length = 45;
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
    mvprintw( 0,2, (char*)(string(VERSION) + " Build " + tool::int2string(BUILDNR)).c_str());

    refresh();

    char *choices[] = {
                          "Unload all modules      ",
                          "Reload all modules      ",
                          "Clear template cache    ",
                          "Run garbage collector   ",
                          "Update sources via CVS  ",
                          "Compile changed sources ",
                          "Recompile all sources   ",
                          "Show source stats       ",
                          "Shut down server"
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
    print( *p_msg );
}

void
ncur::print( string s_msg )
{
    print( (char*)s_msg.c_str() );
}

void
ncur::print( char* c_print )
{
  
    // Removing \n
    if ( strlen(c_print) > i_message_length )
    {
        string s_tmp(c_print);
	print( s_tmp.substr( 0, i_message_length ) );
        print( s_tmp.substr( i_message_length, s_tmp.length()-i_message_length ) ); 
	return;
    }	

    int i;
    char* c_temp = new char[i_message_length];
    memcpy( c_temp, c_print, strlen(c_print) );
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
    int i;

    if( i_choice != 0 )
        switch ( i_choice )
        {
        case 1:
            wrap::MODL->unload_modules();
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Unloaded all modules                             ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 2:
            wrap::MODL->reload_modules();
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Reloaded all modules                            ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 3:
            wrap::HTML->clear_cache();
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Cleared the template cache                                   ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 4:
            if ( ! wrap::GCOL->remove_garbage() )
             wrap::NCUR->print( GAROFFNE );
            pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
            mvprintw( 20,2, "Garbage collector activated                                  ");
            refresh();
            pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
            break;
        case 5:
            tool::shell_command( string(GMAKE) + " cvsupdate 2> error.log");
            break;
        case 6:
            tool::shell_command( string(GMAKE) + " 2> error.log");
            break;
        case 7:
            for ( i = 0; i < GMAKE_ELEMENTS; i++ )
              tool::shell_command( GMAKE + GMAKE_PARAMS[i] + " 2> error.log");
            break;
        case 8:
            tool::shell_command( string(GMAKE) + " stats 2> error.log");
            break;
        case 9: // Shut down server
            if ( ! wrap::GCOL->remove_garbage() )
             wrap::NCUR->print( GAROFFNE );
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
