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
        string*    p_flag     = &(*((map_string*) c->elem[1]))["flag"];
        string*    p_email    = &(*((map_string*) c->elem[1]))["email"];
        string*    p_pass     = &(*((map_string*) c->elem[1]))["pass"];
        string*    p_newpass  = &(*((map_string*) c->elem[1]))["newpass"];
        string*    p_newpass2 = &(*((map_string*) c->elem[1]))["newpass2"];
        user*      p_user     = (user*) c->elem[2];

        if ( *p_flag == "submit" )
        {
         string*  p_msgs  = &(*((map_string*) c->elem[1]))["msgs"];

         // If the password has to be changed:
         if ( *p_pass != "" || *p_newpass != "" || *p_newpass2 != "" ) 
         {
          if ( p_user->get_pass() != *p_pass )
          {
           p_msgs->append( p_lang->get_elem( "ERR_CHANGE_PASSWORD" ) );
           return 0;
          }

          if ( *p_newpass != *p_newpass2 )
          {
           p_msgs->append( p_lang->get_elem( "ERR_CHANGE_PASSWORD2" ) );
           return 0;
          }
          p_user->set_pass( *p_newpass );
	 }

         p_user->set_email( *p_email );
         p_msgs->append( p_lang->get_elem( "OPTIONSCHANGED" ) );
        }

        else
        {
          p_email->append( p_user->get_email() );
        }
 } 
}

