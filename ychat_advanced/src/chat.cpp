// class chat implementation.

#ifndef CHAT_CPP
#define CHAT_CPP

#include "chat.h"
#include "tool.h"

using namespace std;

chat::chat( )
{
    if ( wrap::CONF->get_elem( "HTML" ) == "OFF" )
      b_strip_html = true;

    if ( wrap::CONF->get_elem( "PRINT_ALWAYS_TIME" ) == "YES" )
      b_print_always_time = true;

    if ( wrap::CONF->get_elem( "REPLACE_STRINGS" ) == "YES" )
    {
      b_replace_strings = true;
      vector<string>* p_vec_keys = wrap::CONF->get_key_vector();

      for (vector<string>::iterator iter = p_vec_keys->begin();
           iter != p_vec_keys->end(); iter++ )
      {
       if ( iter->length() >= 9 && iter->compare( 0, 8, "REPLACE_" ) == 0 )
       {
        string s_conf_val = wrap::CONF->get_elem(*iter);
        auto unsigned int i_pos = s_conf_val.find_first_of("||");

        if ( i_pos == string::npos )
         continue;
       
        map_replace_strings[s_conf_val.substr(0, i_pos)] = s_conf_val.substr(i_pos+2); 
       }
     }
      delete p_vec_keys;
    }

    i_max_message_length = tool::string2int(wrap::CONF->get_elem( "MAX_MESSAGE_LENGTH" ));
}

chat::~chat( )
{}

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

    smap<room*,string>::run_func( get_user_, (void*)&param );

    if ( *( (bool*) param.elem[1] ) )
        return (user*) param.elem[2];
}

void
chat::get_user_( room *room_obj, void *v_arg )
{
    container* param = (container*) v_arg;
    if ( *((bool*)param->elem[1]) )
     return; 
    param->elem[2] = (void*)room_obj->get_elem( *((string*)param->elem[0]), *((bool*)param->elem[1]) );
}

void
chat::login( map_string &map_params )
{
    string s_user = map_params["nick"];

    // prove if nick is empty:
    if ( s_user.empty() )
    {
        map_params["INFO"]    = wrap::LANG->get_elem( "ERR_NONICK" );
        map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
        wrap::system_message( LOGINE0 );
        return;
    }

    // prove if the nick is alphanumeric:
    else if ( ! tool::is_alpha_numeric( s_user ) )
    {
        map_params["INFO"]    = wrap::LANG->get_elem( "ERR_ALPNUM" );
        map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
        wrap::system_message( LOGINE1 + s_user  );
        return;
    }

    // prove if the nick is too long:
    else if ( s_user.length()  > tool::string2int( wrap::CONF->get_elem("MAX_NICK_LENGTH") ) ) 
    {
        map_params["INFO"]    = wrap::LANG->get_elem( "ERR_NICK_LENGTH" );
        map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
        wrap::system_message( LOGINE2 + s_user  );
        return;
    }

    // prove if the room name is too long:
    else if ( map_params["room"].length() > tool::string2int( wrap::CONF->get_elem("MAX_ROOMNAME_LENGTH") ) ) 
    {
        map_params["INFO"]    = wrap::LANG->get_elem( "ERR_ROOMNAME_LENGTH" );
        map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
        wrap::system_message( LOGINE3 + s_user + " / " + map_params["room"] );
        return;
    }

    // prove if the room name is valid 
    else if ( map_params["room"].length() < 1 ) 
    {
        map_params["INFO"]    = wrap::LANG->get_elem( "ERR_NOROOM" );
        map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
        wrap::system_message( LOGINE3 + s_user + " / " + map_params["room"] );
        return;
    }

    bool b_flag;

    // prove if nick is already online / logged in.
    get_user( s_user, b_flag );

    if ( b_flag )
    {
	map_params["INFO"]    = wrap::LANG->get_elem( "ERR_ONLINE" ); 
	map_params["request"] = wrap::CONF->get_elem( "STARTMPL" );
			    
	return;
    }

    // Prove if user is recycleable from the garbage collector: 
    user *p_user = wrap::GCOL->get_user_from_garbage( s_user );

    if ( p_user != NULL )
    {
     // 1. possibility to prove the password at login! (using recycled user)
     if ( p_user->get_pass() != map_params["password"] )
     {
      map_params["INFO"]    = wrap::LANG->get_elem( "ERR_WRONG_PASSWORD" );
      map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
      wrap::system_message( LOGINER + s_user );
      return;
     }
     if ( p_user->get_has_sess() )
     {
      map_params["tmpid"] = p_user->get_tmpid();
     }  
     else
     { 
      sess* p_sess = wrap::SMAN->create_session();
      p_sess->set_value( string("nick"), (void *) new string(s_user) );
      map_params["tmpid"] = p_sess->get_id();
      p_user->set_tmpid( map_params["tmpid"] );
      p_user->set_has_sess( true );
     } 
    }

    else //  if ( p_user == NULL ) // If not in garbage create a new user!
    {
     p_user = new user( s_user );

     // prove if nick is registered
     map_string map_results = wrap::DATA->select_user_data( tool::to_lower(s_user), "DATA_SELECT_LOGIN" );

     if ( map_results["nick"] == tool::to_lower(s_user) )
     { 
       p_user->set_is_reg( true );	
       // User exists in database, prove password:
       // 2. possibility to prove the password at login! (using new created user from database)
      if ( map_results["password"] != map_params["password"] )
      {
       map_params["INFO"]    = wrap::LANG->get_elem( "ERR_WRONG_PASSWORD" );
       map_params["request"] = wrap::CONF->get_elem( "STARTMPL" ); // redirect to the startpage.
       wrap::system_message( LOGINER + s_user );
       return;
      }
      else
      { // If registered use saved options
       map_params["registered"] = "yes";
       map_params["color1"] = map_results["color1"];
       map_params["color2"] = map_results["color2"];
       map_params["email"] = map_results["email"];
       map_params["rang"] = map_results["status"];
      }
     }
     else
     { // If not registered use standard font colors
      map_params["color1"] = wrap::CONF->get_elem( "USERCOL1" );
      map_params["color2"] = wrap::CONF->get_elem( "USERCOL2" );
      map_params["rang"] = wrap::CONF->get_elem( "STANDARD_RANG" );
     }

     sess* p_sess = wrap::SMAN->create_session();
     p_sess->set_value( string("nick"), (void *) new string(s_user) );
     map_params["tmpid"] = p_sess->get_id();
     p_user->set_tmpid( map_params["tmpid"] );
     p_user->set_col1( map_params["color1"] );
     p_user->set_col2( map_params["color2"] );
     p_user->set_rang( (rang) tool::string2int(map_params["rang"]));
    // p_user->set_sess( p_sess );
    }

    string s_room = map_params["room"];
    room*  p_room = get_room( s_room , b_flag );

    // if room does not exist add room to list!
    if ( ! b_flag )
    {
     p_room = wrap::GCOL->get_room_from_garbage();

     if ( p_room )
     {
      p_room->set_name( s_room );
      wrap::system_message( REUROOM + s_room );
     }

     else
     {
      p_room = new room( s_room );

      wrap::system_message( NEWROOM + s_room );
     }

     add_elem( p_room );
    }


    // add user to the room.
    p_room->add_user( p_user );

    wrap::system_message( NEWUSER + s_user );

    // post "username enters the chat" into the room.
    string s_msg = wrap::TIMR->get_time() + " "
                 + p_user->get_colored_bold_name()
                 + wrap::LANG->get_elem( "USERENTR" )
                 + "<br>\n";

    // If created a new user from database while logging on (not a recycled user, they already have this set)
    if ( map_params["registered"] == "yes" ) 
    {
     p_user->set_email( map_params["email"] );
     p_user->set_pass( map_params["password"] );
     // Now we will store all wanted user data into MySQL after logging out! (recycled user already have this set)
     p_user->set_changed_data_on(); 
    }
    p_room->msg_post( &s_msg );
}

void
chat::post( user* p_user, map_string &map_params )
{
    string s_msg( map_params["message"] );

    if ( s_msg.length() > i_max_message_length )
    {
     s_msg = s_msg.substr( 0, i_max_message_length );
     string s_private = "<font color=\"" + wrap::CONF->get_elem( "ERRORCOL" ) + "\">"
                       + wrap::LANG->get_elem( "ERR_MESSAGE_LENGTH" ) + "</font><br>\n";
     p_user->msg_post( &s_private );
    } 
    

    if ( b_strip_html )
        tool::strip_html( &s_msg );

    auto unsigned i_pos = s_msg.find( "/" );
    if ( i_pos == 0 )
        return p_user->command( s_msg );

    if ( b_replace_strings )
    {
     for (map_string::iterator iter = map_replace_strings.begin();
          iter != map_replace_strings.end();
          iter++ )
      s_msg = tool::replace( s_msg, iter->first, iter->second );
    }  

    string s_post;

    if ( b_print_always_time )
     s_post.append( wrap::TIMR->get_time() + " " );    

    s_post.append( "<font color=\"#" )
    .append( p_user->get_col1() )
    .append( "\">"              )
    .append( p_user->get_name() )
    .append( ":</font> <font color=\"#")
    .append( p_user->get_col2() )
    .append( "\">"  )
    .append( s_msg              )
    .append( "</font><br>\n"    );

    p_user->get_room()->msg_post( &s_post );
}

#endif
