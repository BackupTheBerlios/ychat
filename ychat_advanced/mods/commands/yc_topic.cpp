#include "../../tool.h"
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
        string s_topic;

        if ( ! params->empty() )
        {
         str_vector::iterator iter;
         s_topic.append( "( " );

         s_msg = p_timr->get_time()
               + " "
               + p_user->get_colored_name()
               + " "
               + p_lang->get_val("TOPIC");

         for ( iter = params->begin(); iter != params->end(); iter++ )
         {
          s_msg.append( " " + *iter );
          s_topic.append( *iter + " " );
         }
         tool::strip_html( &s_topic );
         s_topic.append( ")<br><br>\n" );
        } 

        else
        {
         s_msg = p_timr->get_time()
               + " "
               + p_user->get_colored_name()
               + " "
               + p_lang->get_val("TOPICDELETE");
        }

        s_msg.append( "<script language='JavaScript'>parent.online.location.reload();</script><br>\n" );

        room* p_room = p_user->get_room();

        p_room->msg_post ( &s_msg  );
        p_room->set_topic( s_topic );
 }
}

