#include "../../incl.h"

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

        if ( ! params->empty() )
        {
         str_vector::iterator iter = params->begin();
         string s_searched_user( *iter );
         bool b_found;
         user* p_searched_user = p_chat->get_user( s_searched_user, b_found );
       
         if ( b_found )
         {
          string s_msg         = "<b>" + p_searched_user->get_colored_name() + "</b>"
                               + p_lang->get_elem("USER_IN_ROOM") + "<b>"
                               + p_searched_user->get_room()->get_name() 
          		       + "</b><br>\n";
          p_user->msg_post( &s_msg ); 
         }
      
         else
         {
          conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
          string s_msg = "<font color=\"#" 
                       + p_conf->get_elem("HTML_ERROR_COLOR")
                       + "\"><b>" 
                       + s_searched_user + "</b> "
                       + p_lang->get_elem("ERR_OFFTAVAILABLE")
                       + "</font><br>\n";
          p_user->msg_post( &s_msg ); 

         }
        }
 }

}

