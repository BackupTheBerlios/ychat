// class chat implementation.

#ifndef CHAT_CPP
#define CHAT_CPP

#include "chat.h"
#include "tool.h"

using namespace std;

chat::chat( )
{
    if ( wrap::CONF->get_val( "HTML" ) == "OFF" )
            b_strip_html = true;
    else
        b_strip_html = false;
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

    run_func( get_user_, (void*)&param );

    if ( *( (bool*) param.elem[1] ) )
        return (user*) param.elem[2];
}

void
chat::get_user_( room *room_obj, void *v_arg )
{
    container* param = (container*) v_arg;
    param->elem[2] = (void*)room_obj->get_elem( *((string*)param->elem[0]), *((bool*)param->elem[1]) );
}

void
chat::login( map_string &map_params )
{
    string s_user = map_params["nick"];

    // prove if nick is empty:
    if ( s_user.empty() )
    {
        map_params["INFO"]    = wrap::LANG->get_val( "ERR_NONICK" );
        map_params["request"] = wrap::CONF->get_val( "STARTMPL" ); // redirect to the startpage.
                                    
        return;
    }

    // prove if the nick ist alphanumeric:
    else if ( ! tool::is_alpha_numeric( s_user ) )
    {
        map_params["INFO"]    = wrap::LANG->get_val( "ERR_ALPNUM" );
        map_params["request"] = wrap::CONF->get_val( "STARTMPL" ); // redirect to the startpage.
                                    
        return;
    }

    bool b_flag;

    // prove if nick is already online / logged in.
    get_user( s_user, b_flag );

    if ( b_flag )
    {
        map_params["INFO"]    = wrap::LANG->get_val( "ERR_ONLINE" ); 
        map_params["request"] = wrap::CONF->get_val( "STARTMPL" );
                                    
        return;
    }

    string s_room = map_params["room"];
    room*  p_room = get_room( s_room , b_flag );

    // if room does not exist add room to list!
    if ( ! b_flag )
    {
        p_room = new room( s_room );

#ifdef NCURSES

        string s_tmp( NEWROOM );
        s_tmp.append( s_room );
        wrap::NCUR->
            print( &s_tmp );
#endif

        add_elem( p_room );
    }

    user *p_user = new user( s_user );

    // add user to the room.
    p_room->add_user( p_user );
    sess *ns = wrap::SMAN->create_session();
    ns->setValue(string("nick"), (void *)new string(s_user) );
    map_params["tmpid"]=ns->get_id();

#ifdef NCURSES

    {
        string s_tmp( NEWUSER );
        s_tmp.append( s_user );
        wrap::NCUR->
            print(  s_tmp.c_str()  );
    }
#endif

    // post "username enters the chat" into the room.
    string s_msg = wrap::TIMR->get_time() + " "
                 + p_user->get_colored_name()
                 + wrap::LANG->get_val( "USERENTR" )
                 + "<br>\n";


    p_room->msg_post( &s_msg );
}

void
chat::post( user* p_user, map_string &map_params )
{
    string s_msg( map_params["message"] );
    if ( b_strip_html )
        tool::strip_html( &s_msg );
    auto unsigned i_pos = s_msg.find( "/" );
    if ( i_pos == 0 )
        return p_user->command( s_msg );
    string s_post;

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
