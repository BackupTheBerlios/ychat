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
        wrap::system_message( dlerror() );
        return NULL;
    }

    the_func = (function*) dlsym( the_module, "extern_function" );

    if ( the_func == NULL )
    {
        wrap::system_message( dlerror() );
        return NULL;
    }

    wrap::system_message( MODULEC + s_name ); 

    dynmod *mod     = new dynmod; // encapsulates the function and module handler.
    mod->the_func   = the_func  ; // the function to execute
    mod->the_module = the_module; // the module handler to close if function
    // is not needed anymore.
    map_mods->add_elem  ( mod, s_name  );

    // DO OFFT CLOSE AS LONG THERE EXIST A POINTER TO THE FUNCTION
    // dlclose( module );

    return mod;
}

dynmod*
modl::get_module( string s_name )
{
    wrap::system_message( MODULER + s_name );
    dynmod* mod = map_mods->get_elem( s_name );
    return ! mod ? cache_module( s_name ) : mod;
}

void
modl::unload_modules()
{
    wrap::system_message( MODUNLO );

    // dlclose all the_module's first!
    map_mods->run_func   ( &modl::dlclose_ );
    
    // then clean the hash map.
    map_mods->make_empty ( );
}

void
modl::reload_modules()
{
    unload_modules();

    wrap::system_message( MODRELO );
    preload_modules( new string("mods/commands/") );
    preload_modules( new string("mods/html/") );
}

#endif
