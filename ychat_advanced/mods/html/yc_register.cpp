#include "../../incl.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container* c          = (container*) v_arg;
        dynamic_wrap* p_wrap  = (dynamic_wrap*) c->elem[0];
        lang*      p_lang     = (lang*) p_wrap->LANG; 
        data*      p_data     = (data*) p_wrap->DATA; 
        conf*      p_conf     = (conf*) p_wrap->CONF; 
        string*    p_msgs     = &(*((map_string*) c->elem[1]))["INFO"];
        string*    p_nick     = &(*((map_string*) c->elem[1]))["nick"];
        string*    p_email    = &(*((map_string*) c->elem[1]))["email"];
        string*    p_pass     = &(*((map_string*) c->elem[1]))["pass"];
        string*    p_pass2    = &(*((map_string*) c->elem[1]))["pass2"];
        string*    p_request  = &(*((map_string*) c->elem[1]))["request"];
        user*      p_user     = (user*) c->elem[2];

  	 p_user->set_name( *p_nick );
        *p_nick = tool::to_lower(*p_nick);

        if ( *p_pass != *p_pass2 )
        {
         p_msgs->append( p_lang->get_elem( "ERR_REGISTER_PASSWORD" ) );
        }

        // prove if the nick is alphanumeric:
        else if ( ! tool::is_alpha_numeric( *p_nick ) )
        {
         p_msgs->append( p_lang->get_elem( "ERR_ALPNUM" ) );
        }

        // prove if the nick is too long:
        else if ( p_nick->length() > tool::string2int( p_conf->get_elem("MAX_NICK_LENGTH") ) )
        {
         p_msgs->append( p_lang->get_elem( "ERR_NICK_LENGTH" ) );
        }

        else if ( p_pass->length() > tool::string2int( p_conf->get_elem("MAX_PASS_LENGTH") ) )
        {
         p_msgs->append( p_lang->get_elem( "ERR_PASS_LENGTH" ) );
        }

        else if ( p_email->length() > tool::string2int( p_conf->get_elem("MAX_EMAIL_LENGTH") ) )
        {
         p_msgs->append( p_lang->get_elem( "ERR_PASS_LENGTH" ) );
        }

	else
	{
	 map_string result_map = p_data->select_user_data( *p_nick, "DATA_SELECT_NICK" ); 
	 if (result_map["nick"] == *p_nick)
         {
          p_msgs->append( p_lang->get_elem( "ERR_REGISTER_EXISTS" ) );
         }
	  
	 else
	 {
          p_msgs->append( p_lang->get_elem( "REGISTER_NICK" ) );
          p_request->replace( 0, p_request->length(), p_conf->get_elem("STARTMPL") );

          map_string map_insert = *((map_string*) c->elem[1]);
          map_insert["password"] = *p_pass;
          map_insert["color1"] = p_conf->get_elem("USERCOL1");
          map_insert["color2"] = p_conf->get_elem("USERCOL2");
          map_insert["registerdate"] = tool::int2string((int)tool::unixtime()); 

  	  p_user->set_pass( map_insert["password"] );
  	  p_user->set_email( map_insert["email"] );
  	  p_user->set_col1( map_insert["color1"] );
  	  p_user->set_col2( map_insert["color2"] );

          p_data->insert_user_data( *p_nick, "DATA_REGISTER_NICK", map_insert );
         }
	}	
 } 
}

