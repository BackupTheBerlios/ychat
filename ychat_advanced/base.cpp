#ifndef BASE_CPP
#define BASE_CPP

#include "base.h"

template<class type>
base<type>::base()
{
    map_elem = new smap<type*,string>(80);
}

template<class type>
base<type>::~base( )
{
}

template<class type>
void
base<type>::add_elem( type* p_type )
{
    map_elem->add_elem  ( p_type, p_type->get_name());
}

template<class type>
void
base<type>::del_elem( string &s_name )
{
    map_elem->del_elem  ( s_name	     );
}

template<class type>
type*
base<type>::get_elem( string &s_name, bool &b_found )
{
    type* p_type = map_elem->get_elem( s_name );
    b_found = p_type == NULL ?  false : true;
    return p_type;
}

template<class type>
void
base<type>::run_func( void (*func)(type*, void*), void* v_arg )
{
    map_elem->run_func( func, v_arg );
}

#endif
