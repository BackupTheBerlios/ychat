// smap := Syncronized hmap
// nmap := Syncronized hmap which's get_elem returns a new obj_type
//         instance instead of NULL if for a specific key no value has 
//         been found

#include "incl.h"

#ifndef SMAP_H
#define SMAP_H

#include "hmap.h"

template <class obj_type, class key_type>
class smap : public hmap<obj_type, key_type>
{

 protected:
    pthread_mutex_t mut_smap;

 public:
    smap( double moc );
    ~smap();
    void make_empty();
    void make_empty( void (*func)(key_type) );
    void add_elem ( const obj_type &x, const key_type &k );
    void del_elem ( const key_type &k );
    bool is_avail ( const key_type &k );
    obj_type get_elem ( const key_type &k );
    void run_func( void (*func)(obj_type) );
    void run_func( void (*func)(obj_type, void*), void* v_arg );
    vector<key_type>* get_key_vector();
};

template <class obj_type, class key_type>
class nmap : public smap<obj_type, key_type>
{
 public:
    nmap( double moc );
    ~nmap();

    // Override the get_elem method so that a new object
    // will be returned if the required element is not in
    // the map!
    virtual obj_type get_elem ( const key_type &k ) {
     // Create new object;
     obj_type ret_val;

     pthread_mutex_lock  ( &mut_smap );
     int i_current_pos = find_pos( k );
     if( is_active( i_current_pos ) )
         ret_val = array[ i_current_pos ].element;
 //    else
 //       ret_val = new obj_type();
     pthread_mutex_unlock( &mut_smap );
     return ret_val;
    }
};

#include "smap.cpp"

#endif
