// class name implementation.

#ifndef NAME_CPP
#define NAME_CPP

#include "name.h"

using namespace std;

name::name( string s_name )
{
    set_name( s_name );
    pthread_mutex_init( &mut_s_name, NULL);
}

name::~name()
{
 pthread_mutex_destroy( &mut_s_name );
}

string
name::get_name() 
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_name );
    s_ret = s_name;
    pthread_mutex_unlock( &mut_s_name );
    return s_ret;
}

void
name::set_name( string s_name )
{
//    pthread_mutex_lock  ( &mut_s_name );
    this->s_name = s_name;
//    pthread_mutex_unlock( &mut_s_name );
}


#endif
