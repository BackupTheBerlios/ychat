#ifndef BASE_CPP
#define BASE_CPP

#include "base.h"

template<class type>
base<type>::base() : smap<type*,string>::smap(HMAPOCC)
{
}

template<class type>
base<type>::~base( )
{
}


template<class type>
type*
base<type>::get_elem( string s_name, bool &b_found )
{
    s_name = to_lower(s_name);
    type* p_type = smap<type*,string>::get_elem( s_name );
    b_found = p_type == NULL ?  false : true;
    return p_type;
}

template<class type>
void
base<type>::add_elem( type* p_type )
{
    smap<type*,string>::add_elem(p_type, p_type->get_lowercase_name());
}

#endif
