#ifndef SMAP_CPP
#define SMAP_CPP

#include "smap.h"

template <class obj_type, class key_type>
smap<obj_type, key_type>::smap( double moc ) : hmap<obj_type, key_type>::hmap(moc)
{
 pthread_mutex_init( &mut_smap  , NULL);
}

template <class obj_type, class key_type>
smap<obj_type, key_type>::~smap()
{
 pthread_mutex_destroy( &mut_smap );
}

template <class obj_type, class key_type> void
smap<obj_type, key_type>::make_empty()
{
 pthread_mutex_lock  ( &mut_smap );
 hmap<obj_type,key_type>::make_empty();
 pthread_mutex_unlock( &mut_smap );
}

template <class obj_type, class key_type>
void smap<obj_type, key_type>::make_empty( void (*func)(key_type) )
{
 pthread_mutex_lock  ( &mut_smap );
 hmap<obj_type,key_type>::make_empty( func );
 pthread_mutex_unlock( &mut_smap );
}


template <class obj_type, class key_type>
void smap<obj_type, key_type>::add_elem( const obj_type &x, const key_type &k  )
{
 pthread_mutex_lock  ( &mut_smap );
 hmap<obj_type,key_type>::add_elem( x, k );
 pthread_mutex_unlock( &mut_smap );
}

template <class obj_type, class key_type> void
smap<obj_type, key_type>::del_elem( const key_type & k )
{
 pthread_mutex_lock  ( &mut_smap );
 hmap<obj_type,key_type>::del_elem( k );
 pthread_mutex_unlock( &mut_smap );
}

template <class obj_type, class key_type>
obj_type smap<obj_type, key_type>::get_elem( const key_type &k )
{
 pthread_mutex_lock  ( &mut_smap );
 obj_type ret_val = hmap<obj_type,key_type>::get_elem( k );
 pthread_mutex_unlock( &mut_smap );
 return ret_val;
}

template <class obj_type, class key_type>
bool smap<obj_type, key_type>::is_avail( const key_type &k )
{
 pthread_mutex_lock  ( &mut_smap );
 bool ret_val = hmap<obj_type,key_type>::is_avail( k );
 pthread_mutex_unlock( &mut_smap );
 return ret_val;
}

template<class obj_type, class key_type> void
smap<obj_type, key_type>::run_func( void (*func)(obj_type) )
{
 pthread_mutex_lock  ( &mut_smap );
 hmap<obj_type,key_type>::run_func( func );
 pthread_mutex_unlock( &mut_smap );
}

template<class obj_type, class key_type> void
smap<obj_type, key_type>::run_func( void (*func)(obj_type, void*), void* v_arg )
{
 pthread_mutex_lock  ( &mut_smap );
 hmap<obj_type,key_type>::run_func( func, v_arg );
 pthread_mutex_unlock( &mut_smap );
}

template<class obj_type, class key_type> vector<key_type>*
smap<obj_type, key_type>::get_key_vector()
{
 pthread_mutex_lock  ( &mut_smap );
 vector<key_type>* p_ret_vector = hmap<obj_type,key_type>::get_key_vector();
 pthread_mutex_unlock( &mut_smap );
 return p_ret_vector;
}

template <class obj_type, class key_type>
nmap<obj_type, key_type>::nmap( double moc ) : smap<obj_type, key_type>::smap(moc)
{
}

template <class obj_type, class key_type>
nmap<obj_type, key_type>::~nmap()
{
}

#endif

