#include <iostream>
#include <string>
#include "../../glob.h"
#include "../../hmap.h"
#include "../../lang.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;
void
get_help_strings( void *v_pointer )
{
}

extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container* c= (container*) v_arg;

        // Run get_help_strings( .. ) on each dynmod!
        hmap<dynmod*,string>* map_mods = (hmap<dynmod*,string>*) c->elem[1];
    //    map_mods->run_func( get_help_strings );

        (*((map_string*) c->elem[2]))["content"] = "<td>Will be available soon!</td>";
        // get_help_strings( (lang*) c->elem[0]), (void*) c->elem[1] );
 }
}

