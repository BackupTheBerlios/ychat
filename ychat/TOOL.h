#ifndef TOOL_H
#define TOOL_H

#include "incl.h" 

using namespace std;

class TOOL
{
public:
 static bool is_alpha_numeric( string &s_digit ); 
 static int  string2int( string s_digit ); 
 static long unixtime();
};

#endif
