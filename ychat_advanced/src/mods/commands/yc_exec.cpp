#include "../../incl.h"
#include "../../room.h"
#include "../../user.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

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

        if (p_user->get_rang() == 0)
        {
        
        string s_command;
        string s_msg = p_timr->get_time();
        string s_output;

        str_vector::iterator iter;
        for ( iter = params->begin(); iter != params->end(); iter++ )
         s_command.append( " " + *iter );

        s_command.append(" 2>error.log");

        s_msg.append( " " + s_command + "<br>\n" );

        p_user->msg_post( &s_msg );

        FILE *file;
        char buffer[READBUF];
        char* c_pos;

        if( (file=popen(s_command.c_str(), "r")) == NULL )
        {
          s_output = "<font color=\"#" 
                   + p_conf->get_elem("HTML_ERROR_COLOR")
                   + p_lang->get_elem("ERR_EXEC_COMMAND")
                   + "</font><br>\n";
         p_user->msg_post( &s_output );
        }

        else
        { 
         s_output.append("<hr>\n");
         while(true)
         {
          if(fgets(buffer, READBUF, file) == NULL)
           break;

           s_output.append( string(buffer) + "<br>\n" );
         }

         p_user->msg_post( &s_output );
         pclose(file);
        }
       }
 }
}

