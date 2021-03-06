#ifndef ROOM_CPP
#define ROOM_CPP

#include "room.h"

using namespace std;

room::room( string s_name ) : name( s_name )
{
 pthread_mutex_init( &mut_s_topic, NULL );
 p_logd = new logd( wrap::CONF->get_elem("LOG_ROOM_DIR") + get_lowercase_name(),
                    wrap::CONF->get_elem("LOG_ROOM_LINES") );
}

room::~room()
{
 delete p_logd;
 pthread_mutex_destroy( &mut_s_topic );
 pthread_mutex_destroy( &mut_s_name );
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
    reload_onlineframe();
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

void
room::reload_onlineframe()
{
    string s_msg = "<script language=\"JavaScript\"> parent.online.location.reload(); </script>\n";
    msg_post( &s_msg );
}

void 
room::set_name( string s_name )
{
     if ( tool::to_lower(s_name) == get_lowercase_name() )
     {
      name::set_name( s_name ); 
      return;
     }

     p_logd->flush_logs();

     if ( s_name == "" )
      return;

     name::set_name( s_name ); 
     p_logd->set_logfile( wrap::CONF->get_elem("LOG_ROOM_DIR"), get_lowercase_name() );
} 

#endif
