#ifndef SMAN_CPP
#define SMAN_CPP

#include "sman.h"
#include "tool.h"

sman::sman() : smap<sess*,string>::smap(HMAPOCC)
{
    i_session_count = 0;
    pthread_mutex_init( &mut_i_session_count, NULL );
}

sman::~sman()
{
    pthread_mutex_destroy( &mut_i_session_count );
}

string sman::generate_id( int len )
{
    string valid_chars=".abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMOFFPQRSTUVWXYZ0123456789-_";
    string s_ret = "";

    srand(time(0)+160682);

    for (int i = 0; i < len; i++)
    {
        int i_char= rand() % 64;
        s_ret += valid_chars[i_char];
    }

    return s_ret;
}

sess *sman::create_session( )
{
    string s_id = generate_id( tool::string2int( wrap::CONF->get_elem( "SESSION_LENGTH" ) ) );

    sess* p_sess = new sess( s_id  );

    pthread_mutex_lock( &mut_i_session_count );
    i_session_count++;
#ifdef NCURSES
    print_sessions();
#endif
    pthread_mutex_unlock( &mut_i_session_count );

    add_elem( p_sess, s_id );

    return p_sess;
}

sess *sman::get_session( string s_id )
{
    return get_elem( s_id );
}

void
sman::destroy_session( string s_id )
{
    pthread_mutex_lock( &mut_i_session_count );
    i_session_count--;
#ifdef NCURSES
    print_sessions();
#endif
    pthread_mutex_unlock( &mut_i_session_count );

    sess* p_sess = get_elem( s_id );
    del_elem( s_id );
    delete p_sess;
}

#ifdef NCURSES
void sman::print_sessions()
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    mvprintw( NCUR_SESSION_X,NCUR_SESSION_Y, "Sessions: %d ", i_session_count);
    refresh();
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
}

void sman::print_init_ncurses()
{
    pthread_mutex_lock( &mut_i_session_count );
    print_sessions();
    pthread_mutex_unlock( &mut_i_session_count );
}

int
sman::get_session_count()
{
    int i_ret;
    pthread_mutex_lock( &mut_i_session_count );
    i_ret = i_session_count; 
    pthread_mutex_unlock( &mut_i_session_count );
    return i_ret;
}

#endif
#endif
