#ifndef s_tool_H
#define s_tool_H

#include "incl.h" 

using namespace std;

class s_tool
{
public:
 static bool is_alpha_numeric( string &s_digit ); 
 static int  string2int( string s_digit ); 
 static long unixtime();
 static void strip_html( string *s_str);
};

#endif
