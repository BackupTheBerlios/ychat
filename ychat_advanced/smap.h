// Syncronized hmap
#include "incl.h"

#ifndef SMAP_H
#define SMAP_H

#include "hmap.h"

template <class obj_type, class key_type>
class smap : public hmap<obj_type, key_type>
{

 private:
    pthread_mutex_t mut_smap;

 public:
    smap( double moc );
    ~smap();
    void make_empty();
    void make_empty( void (*func)(key_type) );

    void add_elem ( const obj_type &x, const key_type &k );

    void del_elem ( const key_type &k );
    obj_type get_elem ( const key_type &k );
    void run_func( void (*func)(obj_type) );
    void run_func( void (*func)(obj_type, void*), void* v_arg );
    vector<key_type>* get_key_vector();
};

#include "smap.cpp"

#endif
