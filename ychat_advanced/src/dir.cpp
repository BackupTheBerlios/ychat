#ifndef DIR_CPP
#define DIR_CPP

#include "dir.h"

using namespace std;

dir::dir()
{
}

dir::~dir()
{
 dir_vec.clear();
// if ( dp )
//  delete dp;
 if ( ep )
  delete ep;
}



bool
dir::open_dir( char *c_dir )
{
 return open_dir( *new string( c_dir ) );
}

bool
dir::open_dir( string &s_dir )
{
 dp = opendir( s_dir.c_str() );

 if ( dp == NULL )
  return false; // Could not open dir.

 return true; // Could open dir with success.
}

void
dir::close_dir()
{
 if ( dp != NULL )
  closedir( dp );
}

void
dir::read_dir()
{
 if ( dp != NULL )
  while( ep = readdir( dp ) )
   dir_vec.push_back( string( ep->d_name ) ); 
}

vector<string> 
dir::get_dir_vec()
{
 return dir_vec;
}

#endif
