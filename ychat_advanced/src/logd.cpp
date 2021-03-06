#ifndef LOGD_CPP
#define LOGD_CPP

#include <fstream>
#include "logd.h"

logd::logd( string s_filename, string s_log_lines )
{
    initialize( s_filename, tool::string2int(s_log_lines) );
}

logd::logd( string s_filename, int i_log_lines )
{
    initialize( s_filename, i_log_lines );
}

logd::~logd()
{
    flush();
    pthread_mutex_destroy( &mut_s_logging );
}

void
logd::initialize( string s_filename, int i_log_lines )
{
    pthread_mutex_init( &mut_s_logging, NULL );

    if( s_filename.empty() )
    {
     wrap::system_message( LOGERR2 );
     exit(1);
    }

    s_logfile = s_filename;
    i_lines   = i_log_lines;
}

string
logd::get_time_string()
{
    struct tm *t_m;
    time_t t_cur=time(NULL);
    t_m = gmtime(&t_cur);

    char buffer[100];
    buffer[99] = '\0';
    strftime(buffer, 100, "[%d/%b/%Y:%H:%M:%S %z]", t_m);
    string s_ret = buffer;
    return s_ret; 
}

void 
logd::flush()
{
    ofstream of_output;

    of_output.open(s_logfile.c_str(), ios::app);

    if( of_output == NULL )
    {
        wrap::system_message( LOGERR1 + s_logfile );
        exit(1);
    }


    while( ! s_queue.empty() )
    {
        string s_l=s_queue.front();
        s_queue.pop();
        of_output.write( s_l.c_str(), s_l.size() );
    }

    of_output.close();
}

void
logd::log_access( map_string request )
{
    string s_time = get_time_string();
    string s_logstr = request["REMOTE_ADDR"] + " - - "+s_time+" \"" + request["QUERY_STRING"]+"\" 200 0 \""+request["request"]+"\" \""+request["User-Agent"]+"\"\n";

    pthread_mutex_lock ( &mut_s_logging );
    s_queue.push(s_logstr);
    if( s_queue.size() >= i_lines )
        flush();
    pthread_mutex_unlock( &mut_s_logging );
}

void
logd::log_simple_line( string s_line )
{
    string s_time = get_time_string();
    string s_logstr = s_time + " " + s_line;

    pthread_mutex_lock  ( &mut_s_logging );
    s_queue.push(s_logstr);
    if( s_queue.size() >= i_lines )
     flush();
    pthread_mutex_unlock( &mut_s_logging );
}

void
logd::set_logfile( string s_path, string s_filename )
{
    // Remove "/" from filename!
    unsigned int i_pos = s_filename.find( "/" );
    while ( i_pos != string::npos )
    {
     s_filename.replace( i_pos, 1, "SLASH" );
     i_pos = s_filename.find( "/" );
    }

    // Remove "\" from filename (for non unix systems)!
    i_pos = s_filename.find( "\\" );
    while ( i_pos != string::npos )
    {
     s_filename.replace( i_pos, 1, "BACKSLASH" );
     i_pos = s_filename.find( "\\" );
    }

    pthread_mutex_lock  ( &mut_s_logging ); 
    this->s_logfile = s_path + s_filename;
    pthread_mutex_unlock( &mut_s_logging );
}

void
logd::flush_logs()
{
    pthread_mutex_lock  ( &mut_s_logging ); 
    flush(); 
    pthread_mutex_unlock( &mut_s_logging );
}

#endif
