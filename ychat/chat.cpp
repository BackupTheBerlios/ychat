// class chat implementation.

#ifndef CHAT_CXX
#define CHAT_CXX

#include "chat.h"
#include "CONF.h"
#include "MUTX.h"
#include "TOOL.h"

using namespace std;

chat::chat( )
{
}

chat::~chat( )
{
}

user*
chat::get_user( string &s_user )
{
 bool b_flag;
 return get_user( s_user, b_flag );
}

user*
chat::get_user( string &s_user, bool &b_found )
{
 container param;

 param.elem[0] = (void*) &s_user ;
 param.elem[1] = (void*) &b_found;

 b_found = false;

 run_func( get_user_, (void*)&param ); 

 if ( *( (bool*)param.elem[1] ) )
  return (user*)param.elem[2];
}

void
chat::get_user_( name *name_obj, void *v_arg )
{
 container* param = (container*) v_arg;
 room *room_obj = static_cast<room*>(name_obj); 
 param->elem[2] = (void*)room_obj->get_elem( *((string*)param->elem[0]), *((bool*)param->elem[1]) ); 
}

void
chat::login( map_string &map_params )
{
 string s_user = map_params["nick"];

 // prove if nick is empty: 
 if ( s_user.empty() )
 {
  map_params["INFO"]    = E_NONICK; 
  map_params["request"] = CONF::get().get_val( "STARTMPL" ); // redirect to the startpage.
  return;
 }

 // prove if the nick ist alphanumeric:
 else if ( ! TOOL::is_alpha_numeric( s_user ) )
 {
  map_params["INFO"]    = E_ALPNUM; 
  map_params["request"] = CONF::get().get_val( "STARTMPL" ); // redirect to the startpage.
  return;
 }

 bool b_flag;

 // prove if nick is already online / logged in.
 get_user( s_user, b_flag );
 if ( b_flag ) 
 {
  map_params["INFO"]    = E_ONLINE;
  map_params["request"] = CONF::get().get_val( "STARTMPL" );
  return;
 } 

 string s_room = map_params["room"]; 
 room*  p_room = get_room( s_room , b_flag );

 // if room does not exist add room to list!
 if ( ! b_flag )
 {
  p_room = new room( s_room );

#ifdef _VERBOSE
  pthread_mutex_lock  ( &MUTX::get().mut_stdout );
  cout << NEWROOM << s_room << endl;
  pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
  
  add_elem( p_room );
 }

 user *p_user = new user( s_user );

 // add user to the room.
 p_room->add_user( p_user );

 // post "username enters the chat" into the room.
 p_room->msg_post( new string( s_user.append( USERENTR ) ) );  

#ifdef _VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << LOGINPR << s_user << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
}

void 
chat::post( user* p_user, map_string &map_params )
{

 string s_msg( map_params["message"] );

 auto unsigned i_pos = s_msg.find( "/" );
 if ( i_pos == 0 )
  return p_user->command( s_msg ); 

 string s_post( "<font color=\""   );

 s_post.append( p_user->get_col1() )
       .append( "\">"              )
       .append( p_user->get_name() ) 
       .append( ": "               )
       .append( s_msg              )
       .append( "</font><br>\n"    );

 p_user->get_p_room()->msg_post( &s_post ); 
}

#endif
