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
}

void
logd::initialize( string s_filename, int i_log_lines )
{
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
    ofstream s_output;
    s_output.open(s_logfile.c_str(), ios::app);

    if( s_output == NULL )
    {
        wrap::system_message( LOGERR1 + s_logfile );
        exit(1);
    }


    while(!s_queue.empty())
    {
        string s_l=s_queue.front();
        s_queue.pop();
        s_output.write(s_l.c_str(), s_l.size());

    }
    s_output.close();
}

void
logd::log_access( map_string request )
{
    string s_time = get_time_string();
    string s_logstr = request["REMOTE_ADDR"] + " - - "+s_time+" \"" + request["QUERY_STRING"]+"\" 200 0 \""+request["request"]+"\" \""+request["User-Agent"]+"\"\n";

    s_queue.push(s_logstr);

    if( s_queue.size() >= i_lines )
        flush();
}

void
logd::log_simple_line( string s_line )
{
    string s_time = get_time_string();
    string s_logstr = s_time + " " + s_line + "\n"; 

    s_queue.push(s_logstr);

    if( s_queue.size() >= i_lines )
     flush();
}






#endif
