#ifndef GCOL_CPP
#define GCOL_CPP

#include "gcol.h"

using namespace std;

gcol::gcol()
{
#ifdef NCURSES
 wrap::NCUR->print( GARBAGE );
#endif
#ifdef SERVMSG
 cout << GARBAGE << endl;
#endif
 pthread_mutex_init( &mut_vec_rooms  , NULL);
 pthread_mutex_init( &mut_vec_users  , NULL);
}

gcol::~gcol()
{
 remove_garbage();
 pthread_mutex_destroy( &mut_vec_rooms );
 pthread_mutex_destroy( &mut_vec_users );
}

void
gcol::add_room_to_garbage( room* p_room )
{ 
 pthread_mutex_lock  ( &mut_vec_rooms ); 
 p_vec_rooms.push_back( p_room ); 
 pthread_mutex_unlock( &mut_vec_rooms );
#ifdef NCURSES
 wrap::NCUR->print( GARROOM + p_room->get_name() );
#endif
#ifdef SERVMSG
 cout << GARROOM + p_room->get_name() << endl;
#endif
}

void
gcol::add_user_to_garbage( user* p_user )
{ 
 pthread_mutex_lock  ( &mut_vec_users ); 
 p_vec_users.push_back( p_user ); 
 pthread_mutex_unlock( &mut_vec_users );
#ifdef NCURSES
 wrap::NCUR->print( GARUSER + p_user->get_name() );
#endif
#ifdef SERVMSG
 cout << GARUSER + p_user->get_name() << endl;
#endif
}

void
gcol::remove_garbage()
{
#ifdef NCURSES
 wrap::NCUR->print( GARBACT );
#endif
#ifdef SERVMSG
 cout << GARBACT << endl;
#endif
 pthread_mutex_lock  ( &mut_vec_rooms );
 for ( vector<room*>::iterator iter = p_vec_rooms.begin();
       iter != p_vec_rooms.end(); iter++ )
  delete *iter;
 p_vec_rooms.clear();
 pthread_mutex_unlock( &mut_vec_rooms );

 pthread_mutex_lock  ( &mut_vec_users );
 for ( vector<user*>::iterator iter = p_vec_users.begin();
       iter != p_vec_users.end(); iter++ )
  delete *iter;
 p_vec_users.clear();
 pthread_mutex_unlock( &mut_vec_users );
}

#endif
