// class conf implementation.

#ifndef CONF_CPP
#define CONF_CPP

#include <fstream>
#include "conf.h"

using namespace std;

conf::conf( string s_conf = CONFILE ) : nmap<string,string>::nmap(HMAPOCC), name::name( s_conf )
{
    parse( ); // parse the config file.
}

conf::~conf()
{}

void
conf::parse()
{
#ifdef VERBOSE
    wrap::system_message( CFILEOK + get_name() );
#endif

    string s_configfile = get_name();

    string s_homeconfig = string(getenv("HOME"))+string("/.ychat/")+get_name();
    ifstream if_homecheck( s_homeconfig.c_str() );

    if( if_homecheck )
    {
        if_homecheck.close();
        s_configfile = s_homeconfig;
    }

    ifstream fs_conf( s_configfile.c_str() );

    if ( ! fs_conf )
    {
#ifdef VERBOSE
        wrap::system_message( CFILEOFF + get_name() );
#endif
        return;
    }

    char c_buf[READBUF];

    while( fs_conf.getline( c_buf, READBUF ) )
    {
        string s_token( c_buf );
        unsigned int ui_pos = s_token.find( "#", 0 );

        // if line is commented out:
        if ( ui_pos == 0 )
            continue;

        ui_pos = s_token.find( ";", 0 );

        // if token has not been found.
        if ( ui_pos == string::npos )
            continue;

        s_token = s_token.substr( 0   , --ui_pos );
        ui_pos  = s_token.find  ( "\"", 0        );

        if ( ui_pos == string::npos )
            continue;

        string s_val = s_token.substr( ui_pos+1, s_token.length() );
        string s_key = s_token.substr( 0      , --ui_pos          );

        // fill the map.
        nmap<string,string>::add_elem(s_val, s_key); //map_vals[s_key] = s_val;
    }

    fs_conf.close();
    //fs_conf.~ifstream();
}

#endif
