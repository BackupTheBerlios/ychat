#ifndef MODL_CPP
#define MODL_CPP

#include <limits.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "modl.h"
#include "dir.h"

using namespace std;

modl::modl(  )
{
    map_mods = new smap<dynmod*,string>(HMAPOCC);

    if ( wrap::CONF->get_elem( "PRE_MODS_COMMANDS" ).compare( "ON" ) == 0 )
     preload_modules( new string("mods/commands/") );

    if ( wrap::CONF->get_elem( "PRE_MODS_HTML" ).compare( "ON" ) == 0 )
     preload_modules( new string("mods/html/") );
}

modl::~modl()
{
    // dlclose all the_module's first!
    map_mods->run_func   ( &modl::dlclose_ );

    // then clean the hash map.
    unload_modules(); 
}

void
modl::preload_modules( string *p_path )
{

 dir* p_dir = new dir();
 p_dir->open_dir( *p_path );

 p_dir->read_dir();
 
 vector<string> dir_vec = p_dir->get_dir_vec();

 if ( ! dir_vec.empty() ) 
 {
  vector<string>::iterator iter = dir_vec.begin();

  do
  {
   if ( iter->length() >= 3 && iter->compare( iter->length()-3, 3, ".so" ) == 0 )
    cache_module( *p_path + *iter );
  }
  while ( ++iter != dir_vec.end() );
 }

 dir_vec.clear();
 p_dir->close_dir();
 delete p_dir;
 delete p_path;
}

void
modl::dlclose_( dynmod* mod )
{
    dlclose( mod->the_module );
    free   ( mod );
}

dynmod*
modl::cache_module( string s_name )
{
    void     *the_module = NULL;
    function *the_func   = NULL;

    the_module = dlopen( s_name.c_str(), RTLD_NOW );

    if ( the_module == NULL )
    {
        pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );

#ifdef NCURSES
        wrap::NCUR->print( dlerror() );
#endif
#ifdef SERVMSG
        cerr << "dlerror: " << dlerror() << endl;
#endif

        pthread_mutex_unlock( &wrap::MUTX->mut_stdout ); 
        return NULL;
    }

    the_func = (function*) dlsym( the_module, "extern_function" );

    if ( the_func == NULL )
    {
        pthread_mutex_lock  ( &wrap::MUTX->mut_stdout ); 

#ifdef NCURSES
        wrap::NCUR->print( dlerror() );
#endif
#ifdef SERVMSG
        cerr << "dlerror: " << dlerror() << endl;
#endif

        pthread_mutex_unlock( &wrap::MUTX->mut_stdout ); 
        return NULL;
    }

#ifdef SERVMSG
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout ); 
    cout << MODULEC << s_name << endl;
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout ); 
#endif
#ifdef NCURSES
    string s_tmp( MODULEC );
    s_tmp.append( s_name );
    wrap::NCUR->print( s_tmp.c_str() );
#endif

    dynmod *mod     = new dynmod; // encapsulates the function and module handler.
    mod->the_func   = the_func  ; // the function to execute
    mod->the_module = the_module; // the module handler to close if function
    // is not needed anymore.
    map_mods->add_elem  ( mod, s_name  );

    // DO NOT CLOSE AS LONG THERE EXIST A POINTER TO THE FUNCTION
    // dlclose( module );

    return mod;
}

dynmod*
modl::get_module( string s_name )
{
#ifdef VERBOSE
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout ); 
    cout << MODULER << s_name << endl;
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout ); 
#endif
#ifdef NCURSES
    string s_tmp( MODULER );
    s_tmp.append( s_name );
    wrap::NCUR->print( s_tmp.c_str() );
#endif
    dynmod* mod = map_mods->get_elem( s_name );

    return ! mod ? cache_module( s_name ) : mod;
}

void
modl::unload_modules()
{
#ifdef NCURSES
    wrap::NCUR->print( MODUNLO );
#endif
#ifdef SERVMSG
    cout << MODUNLO << endl;
#endif

    // dlclose all the_module's first!
    map_mods->run_func   ( &modl::dlclose_ );
    
    // then clean the hash map.
    map_mods->make_empty ( );
}

void
modl::reload_modules()
{
     unload_modules();
#ifdef NCURSES
    wrap::NCUR->print( MODRELO );
#endif
#ifdef SERVMSG
    cout << MODRELO << endl;
#endif
     preload_modules( new string("mods/commands/") );
     preload_modules( new string("mods/html/") );
}

#endif
