#ifndef TOOL_H
#define TOOL_H

#include "incl.h"

using namespace std;

class tool
{
public:
    static bool is_alpha_numeric( string &s_digit );
    static int  string2int( string s_digit );
    static char* int2char( int i_int );
    static string int2string( int i_int );
    static string trim( string s_str );
    static long unixtime();
    static void strip_html( string *s_str)
    {
     auto int i_pos;
     if((i_pos=s_str->find("<",0))==string::npos)
      return;
     while(true)
     {
      s_str->replace(i_pos,1,"&lt;");
      if((i_pos=s_str->find("<",0))==string::npos)
       return;
     }
    }

    static string getExtension( string s_file );
    static string to_lower( string s );
};

#endif
