#include "../../user.h"
#include "../../room.h"

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);
 string to_upper( string &s_msg )
 {
  char* c_msg = (char*) s_msg.c_str();
  
  for ( int i = 0; i < strlen( c_msg ); i++ )
   c_msg[i] = toupper( c_msg[i] );
 
  return string( c_msg ); 
 };
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
	str_vector *params= (str_vector*) c->elem[2];	// param array

        lang* p_lang = (lang*) ((dynamic_wrap*)c->elem[3])->LANG; 

        string s_msg = "<i>" + p_user->get_colored_name() + " "
                       + p_lang->get_val("SCREAM")
                       + " <font color=\"#" + p_user->get_col2() 
                       + "\">";
        if ( ! params->empty() )
        {
         str_vector::iterator iter = params->begin();
         for ( iter = params->begin(); iter != params->end(); iter++ )
          s_msg.append( to_upper(*iter) + " " );
        }

        s_msg.append( "</font></i><br>\n" );
        p_user->get_room()->msg_post( &s_msg );

 }
}

