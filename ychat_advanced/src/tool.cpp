#ifndef TOOL_CPP
#define TOOL_CPP

#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#include "tool.h"

string
tool::trim( string s_str )
{
    if(s_str.empty())
        return "";

    char c_cur = s_str[0];
    auto int pos=0;

    // left trim
    while (c_cur==' '|| c_cur == '\n' || c_cur == '\r')
    {
        s_str.erase(pos,1);
        c_cur = s_str[++pos];
    }

    // right trim
    pos=s_str.size();
    c_cur = s_str[s_str.size()];

    while (c_cur==' ' || c_cur == '\n' || c_cur == '\0' || c_cur == '\r')
    {
        s_str.erase(pos,1);
        c_cur = s_str[--pos];
    }

    return s_str;
}

char*
tool::clean_char( char* c_str )
{
 int i_len = strlen(c_str);

 if(i_len == 0)
  return c_str;

 char* c_pos = c_str;

 while ( *c_pos != '\0' )
 {
  if ( iscntrl(*c_pos) )
    *c_pos = ' ';
  c_pos++;
 }
 return c_str;
}

string
tool::get_extension( string s_file )
{
    int pos = s_file.find_last_of(".");
    if(pos != string::npos)
    {
        string s_ext=s_file.substr(pos+1, s_file.size()-(pos+1));
        for(int i = 0;i<s_ext.size();i++)
            s_ext[i]=toupper(s_ext[i]);
        return s_ext;
    }
    return "";
}

char*
tool::int2char( int i_int )
{
    char *buffer = new char[64];
    sprintf(buffer, "%d", i_int);
    return buffer;
}

void
tool::shell_command( string s_command )
{
    FILE *file;
    char buffer[READBUF];
    char* c_pos;

    wrap::system_message(SHELLEX);
    wrap::system_message(s_command);

    if( (file=popen(s_command.c_str(), "r")) == NULL )
    {
     wrap::system_message( SHELLER );
    }

    else
    { 
     while(true)
     {
      if(fgets(buffer, READBUF, file) == NULL)
       break;
      
      wrap::system_message( clean_char(buffer) );
     }
     pclose(file);
    }
}

#endif

