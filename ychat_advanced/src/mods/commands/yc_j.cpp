#include "../../user.h"
#include "../../room.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);
 
 int extern_function(void *v_arg)
 {

	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
	str_vector *params = (str_vector*) c->elem[2];	// param array
        chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT; 
        lang* p_lang = (lang*) ((dynamic_wrap*)c->elem[3])->LANG; 
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 
        gcol* p_gcol = (gcol*) ((dynamic_wrap*)c->elem[3])->GCOL; 

        if ( ! params->empty() )
        {
         str_vector::iterator iter; 

         string s_room;

         for ( iter = params->begin(); iter != params->end(); iter++ )
         {
          s_room.append( *iter ); 
          if ( iter+1 != params->end() )
           s_room.append(" ");
         }

         int i_max_room_len = tool::string2int( p_conf->get_elem("MAX_ROOMNAME_LENGTH") ); 
         if ( i_max_room_len < s_room.length() )
          s_room = s_room.substr(0, i_max_room_len );         

         if ( tool::to_lower(s_room) == p_user->get_room()->get_lowercase_name() )
         {
          string s_msg = "<font color=\"" + p_conf->get_elem("ERRORCOL") + "\">"
		       + p_lang->get_elem("ERR_ALREADY_IN_ROOM") + "<b>"
                       + p_user->get_room()->get_name() + "</b></font><br>\n";	
          p_user->msg_post( &s_msg );
         }

         else
         {
      
          string s_user = p_timr->get_time() + " "
                        + p_user->get_colored_bold_name(); 
          string s_msg = s_user 
                       + p_lang->get_elem( "USER_LEAVES_ROOM" )
	               + "<b>" + s_room + "</b><br>\n";

          string s_msg2 = s_user
                        + p_lang->get_elem( "USER_ENTERS_ROOM" )
	                + s_room + "<br>\n";

	  bool b_flag;
          room*  p_room = p_chat->get_room( s_room , b_flag );

          // if room does not exist add room to list!
          if ( p_room == NULL )
          {
           p_room = p_gcol->get_room_from_garbage_or_new();
           p_room->set_name( s_room );

           room* p_room_old = p_user->get_room();

           string s_name = p_user->get_name();
           p_user->get_room()->del_elem( s_name );

           if ( p_room_old != NULL )
 	    p_room_old->msg_post( &s_msg ); 

           p_room->add_user( p_user );
           p_chat->add_elem( p_room );
           p_room->msg_post( &s_msg2 );
          }

          else // p_room != NULL
          {
           // add user to the room.
 	   p_user->get_room()->msg_post( &s_msg ); 
           string s_name = p_user->get_name();
           p_user->get_room()->del_elem( s_name );
           p_room->add_user( p_user );
           p_room->msg_post( &s_msg2 );
	  }
         }
	}
 }

}

