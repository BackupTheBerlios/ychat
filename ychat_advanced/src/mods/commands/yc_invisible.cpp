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
       
        bool b_invisible = p_user->get_invisible(); 
 
        string s_msg = b_invisible
                     ? p_lang->get_elem("UNSET_MODE_INVISIBLE") 
                     : p_lang->get_elem("SET_MODE_INVISIBLE") 
                     + "<br>\n"; 

        p_user->set_invisible( !b_invisible );
        p_user->get_room()->reload_onlineframe();
        p_user->msg_post ( &s_msg  );
 }
}

