#ifndef s_tool_CXX
#define s_tool_CXX

#include <ctype.h> 
#include <time.h>
#include "s_tool.h"

bool
s_tool::is_alpha_numeric( string &s_digit )
{
 auto const char *digit = s_digit.c_str();
 auto int   i_len = strlen( digit );

 for( int i=0; i<i_len; i++ )
 {
  if ( ! isalnum( *digit ) )
   return false;
  digit++;
 }

 return true;
}

int
s_tool::string2int( string s_digit )
{
 auto const char *digit = s_digit.c_str();
 int result = 0;

 // Convert each digit char and add into result.
 while (*digit >= '0' && *digit <='9') {
  result = (result * 10) + (*digit - '0');
  digit++;
 }

 // Check that there were no non-digits at end.
 if (*digit != 0) {
  return -1;
 }

 return result;
}

long
s_tool::unixtime()
{
 return (long) time( NULL );
}

#endif
