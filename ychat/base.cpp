/*
	This file is part of yChat

	$Author: paul $
	$Date: 2003/03/30 00:14:09 $
	
	$Header: /home/xubuntu/berlios_backup/github/tmp-cvs/ychat/Repository/ychat/base.cpp,v 1.6 2003/03/30 00:14:09 paul Exp $
*/
// template class data implementation;

#ifndef BASE_CPP
#define BASE_CPP

#include "base.h"

template<class type>
base<type>::base()
{
 map_elem = new hmap<name*,string>(80);
 pthread_mutex_init (&mut_map_elem, NULL ); 
}

template<class type>
base<type>::~base( )
{
 pthread_mutex_destroy( &mut_map_elem );
}

template<class type> void
base<type>::add_elem( name* p_name )
{
 pthread_mutex_lock  ( &mut_map_elem   );
 map_elem->add_elem  ( p_name          ); 
 pthread_mutex_unlock( &mut_map_elem   );
}

template<class type> void
base<type>::del_elem( string &s_name )
{
 pthread_mutex_lock  ( &mut_map_elem );
 map_elem->del_elem  ( s_name	     ); 
 pthread_mutex_unlock( &mut_map_elem );
}

template<class type> name*
base<type>::get_elem( string &s_name, bool &b_found )
{
 pthread_mutex_lock  ( &mut_map_elem );
 name* p_name = map_elem->get_elem( s_name ); 
 pthread_mutex_unlock( &mut_map_elem );

 b_found = p_name == NULL ?  false : true;

 return p_name; 
}

template<class type> void
base<type>::run_func( void (*func)(name*, void*), void* v_arg )
{
 pthread_mutex_lock  ( &mut_map_elem );
 map_elem->run_func( func, v_arg ); 
 pthread_mutex_unlock( &mut_map_elem );
}

#endif
