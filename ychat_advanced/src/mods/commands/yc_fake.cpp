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
       
        bool b_fake = p_user->get_fake(); 
 
        string s_msg = b_fake
                     ? p_lang->get_elem("UNSET_MODE_FAKE") 
                     : p_lang->get_elem("SET_MODE_FAKE")
                     + "<br>\n"; 

        p_user->set_fake( !b_fake );
        p_user->get_room()->reload_onlineframe();
        p_user->msg_post ( &s_msg  );
 }
}

