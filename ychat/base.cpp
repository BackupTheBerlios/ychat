/*
	This file is part of yChat

	$Author: volker $
	$Date: 2003/03/28 00:38:46 $
	
	$Header: /home/xubuntu/berlios_backup/github/tmp-cvs/ychat/Repository/ychat/base.cpp,v 1.5 2003/03/28 00:38:46 volker Exp $
*/
// template class data implementation;

#ifndef BASE_CPP
#define BASE_CPP

#include "base.h"

template<class type>
base<type>::base()
{
 pthread_mutex_init (&mut_vec_elem, NULL ); 
}

template<class type>
base<type>::~base( )
{
 pthread_mutex_destroy( &mut_vec_elem );
}

template<class type> void
base<type>::add_elem( name* p_name )
{
 pthread_mutex_lock  ( &mut_vec_elem   );
 vec_elem.push_back  ( p_name          ); 
 pthread_mutex_unlock( &mut_vec_elem   );
}

template<class type> bool
base<type>::del_elem( string &s_name )
{
 vector<name*>::iterator iter;
 pthread_mutex_lock  ( &mut_vec_elem );

 iter = vec_elem.begin();
 while( iter != vec_elem.end() )
 {
   if ( (*iter)->get_name() == s_name ) 
   {
    vec_elem.erase( iter );
    pthread_mutex_unlock( &mut_vec_elem );
    return true;
   }
   iter++;
 }

 pthread_mutex_unlock( &mut_vec_elem );
 return false;
}

template<class type> name*
base<type>::get_elem( string &s_name, bool &b_found )
{
 vector<name*>::iterator iter;
 pthread_mutex_lock  ( &mut_vec_elem );

 iter = vec_elem.begin();
 while( iter != vec_elem.end() )
 {
   if ( (*iter)->get_name() == s_name ) 
   {
    b_found = true;
    pthread_mutex_unlock( &mut_vec_elem );
    return (*iter);
   }
   iter++;
 }

 pthread_mutex_unlock( &mut_vec_elem );

 b_found = false;

 return new name(); 
}

template<class type> void
base<type>::run_func( void (*func)(name*, void*), void* v_arg )
{
 vector<name*>::iterator iter;
 pthread_mutex_lock  ( &mut_vec_elem );

 // execute func foreach element of vec_elem with 
 // 1st argument: a pointer of a element of vec_elem.
 // 2nd argument: a void pointer of a object.
 for( iter = vec_elem.begin(); iter != vec_elem.end(); iter++ )
   ( *func ) ( (*iter), v_arg );
 
 pthread_mutex_unlock( &mut_vec_elem );
}

#endif
