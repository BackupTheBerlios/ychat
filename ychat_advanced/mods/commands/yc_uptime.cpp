#include "../../incl.h"

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
        lang* p_lang = (lang*) ((dynamic_wrap*)c->elem[3])->LANG; 
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 
        
        string s_msg = p_lang->get_val("UPTIME") + p_timr->get_uptime() + "<br>\n"; 

        p_user->msg_post( &s_msg );
 }
}

