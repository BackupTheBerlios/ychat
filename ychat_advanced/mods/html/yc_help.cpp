#include "../../incl.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container* c         = (container*) v_arg;
        dynamic_wrap* p_wrap = (dynamic_wrap*) c->elem[0];
        modl*      p_modl    = (modl*) p_wrap->MODL; 
        lang*      p_lang    = (lang*) p_wrap->LANG; 
        string*    s_content = &(*((map_string*) c->elem[1]))["content"];

        vector<string>* p_vec_keys = p_modl->get_mod_vector();
          
  	
	sort(p_vec_keys->begin(), p_vec_keys->end());
	vector<string>::iterator p_vec_keys_iter = p_vec_keys->begin();

        while( p_vec_keys_iter != p_vec_keys->end() ) {

         if ( p_vec_keys_iter->find( "mods/commands/", 0 ) != string::npos )
         {

          s_content->append( "<tr><td>" );
          string s_helptext = p_lang->get_elem( *p_vec_keys_iter ) ;

          unsigned int i_pos = s_helptext.find( "||", 0 ); 
          if ( i_pos != string::npos )
           s_helptext.replace( i_pos, 2, "</td><td>" );

          s_content->append( s_helptext );
          s_content->append( "</td></tr>\n" );

         }
         p_vec_keys_iter++;
        }
 }
}

