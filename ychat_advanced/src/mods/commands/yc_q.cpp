#include <iostream>
#include "../../user.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;


extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container* c = (container*) v_arg;
	string* s_command_line = (string*) c->elem[0];
	user* p_user = (user*)c->elem[1];

        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 

  	string s_quit = "<script language=JavaScript>top.location.href='/"
                      + p_conf->get_elem("HTML_START_SITE")
                      + "';</script>";

	p_user->msg_post( &s_quit );
        p_user->set_online(false);

        delete s_command_line;
 }
}

