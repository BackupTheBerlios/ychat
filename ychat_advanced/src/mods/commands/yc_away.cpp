#include "../../incl.h"
#include "../../room.h"
#include "../../user.h"

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user* p_user = (user*) c->elem[1];		// the corresponding user
        lang* p_lang = (lang*) ((dynamic_wrap*)c->elem[3])->LANG; 
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 
	str_vector *params = (str_vector*) c->elem[2];	// param array
        
        string s_msg;
        string s_away;
        string s_col;

        str_vector::iterator iter;

        string s_time = p_timr->get_time();

        s_away.append( s_time ).append( " " );

        s_msg = s_time
              + " "
              + p_user->get_colored_bold_name()
              + " "
              + p_lang->get_elem("SETAWAY")
              + "<font color=" + p_user->get_col2() + ">"; 

        for ( iter = params->begin(); iter != params->end(); iter++ )
        {
         s_msg.append( " " + *iter );
         s_away.append( *iter + " " );
        }
        s_msg.append( "</font><br>\n" );

        room* p_room = p_user->get_room();

        // Remove ' from away message:
        auto unsigned int pos;

        do {
         pos = s_away.find_first_of( "'" );
         if ( pos == string::npos )
          break;
         s_away.replace( pos, 1, "\"" );
         } while (true);

        p_user->set_away( true, s_away );
        p_room->reload_onlineframe();
        p_room->msg_post ( &s_msg  );
 }
}

