#include "incl.h"

#ifndef MODL_H
#define MODL_H

#include "hmap.h"

using namespace std;

class modl
{
private:
    hmap<dynmod*,string>* map_mods;
    pthread_mutex_t mut_map_mods;

    static  void dlclose_( dynmod* mod   );
    dynmod* cache_module ( string s_name );
    void preload_modules( string *p_path );

public:
    modl();
    ~modl();

    dynmod* get_module( string s_name );

    vector<string>* get_mod_vector()
    {
     pthread_mutex_lock  ( &mut_map_mods );
     vector<string>* p_ret = map_mods->get_key_vector(); 
     pthread_mutex_unlock( &mut_map_mods );
     return p_ret;
    }

    void unload_modules(); 
    void reload_modules(); 
};

#endif
