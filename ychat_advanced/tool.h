#ifndef TOOL_H
#define TOOL_H

#include "incl.h"

using namespace std;

class tool
{
public:
    static bool is_alpha_numeric( string &s_digit );
    static char* int2char( int i_int );
    static string int2string( int i_int );
    static string trim( string s_str );
    static long unixtime();
    static int string2int( string s_digit )
    {
     auto const char *digit = s_digit.c_str();
     int result = 0;

     // Convert each digit char and add into result.
     while (*digit >= '0' && *digit <='9')
     {
         result = (result * 10) + (*digit - '0');
         digit++;
     }

     // Check that there were no non-digits at end.
     if (*digit != 0)
     {
        return -1;
     }

     return result;
    }
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

    static string get_extension( string s_file );
    static string to_lower( string s );
};

#endif
