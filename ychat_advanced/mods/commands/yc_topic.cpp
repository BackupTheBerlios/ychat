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
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
	str_vector *params = (str_vector*) c->elem[2];	// param array
        
        string s_msg;
        string s_topic;
        string s_col;

        if ( ! params->empty() )
        {
         str_vector::iterator iter;

         s_col = "<font color=" + p_user->get_col2() + ">"; 

         s_msg = p_timr->get_time()
               + " "
               + p_user->get_colored_name()
               + " "
               + p_lang->get_elem("TOPIC")
               + s_col;

         for ( iter = params->begin(); iter != params->end(); iter++ )
         {
          s_msg.append( " " + *iter );
          s_topic.append( *iter + " " );
         }
         s_msg.append( "</font>" );

         int i_topic_length = tool::string2int(p_conf->get_elem("ROOM_TOPIC_LENGTH"));
         if ( s_topic.length() > i_topic_length )
         {
          s_topic = s_topic.substr(0, i_topic_length );
          s_topic.append(" ... ");

          string s_private_msg = "<font color=#" + p_conf->get_elem("ERRORCOL") 
           + ">" + p_lang->get_elem( "TOPICCUT" ) + "</font><br>";

          p_user->msg_post ( &s_private_msg ); 
         }

         s_topic = "( " + s_col + s_topic + "</font>)<br><br>";
        } 

        else
        {
         s_msg = p_timr->get_time()
               + " "
               + p_user->get_colored_name()
               + " "
               + p_lang->get_elem("TOPICDELETE");
        }

        s_msg.append( "<script language='JavaScript'>parent.online.location.reload();</script><br>\n" );

        room* p_room = p_user->get_room();

        p_room->set_topic( s_topic );
        p_room->msg_post ( &s_msg  );
 }
}

