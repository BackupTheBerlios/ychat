#ifndef ROOM_CPP
#define ROOM_CPP

#include "room.h"

using namespace std;

room::room( string s_name ) : name( s_name )
{
 pthread_mutex_init( &mut_s_topic, NULL );
}

room::~room()
{
 pthread_mutex_destroy( &mut_s_topic );
}

string
room::get_topic()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_topic );
    s_ret = s_topic;
    pthread_mutex_unlock( &mut_s_topic );
    return s_ret;
}

void
room::set_topic( string s_topic)
{
    pthread_mutex_lock  ( &mut_s_topic );
    this->s_topic = s_topic;
    pthread_mutex_unlock( &mut_s_topic );
}

void
room::clean_room()
{
    pthread_mutex_lock  ( &mut_s_topic );
    this->s_topic = "";
    pthread_mutex_unlock( &mut_s_topic );
    wrap::CHAT->del_elem( get_lowercase_name() );
    wrap::GCOL->add_room_to_garbage( this ); 
}

#endif
