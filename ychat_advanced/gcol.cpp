#ifndef GCOL_CPP
#define GCOL_CPP

#include "gcol.h"

using namespace std;

gcol::gcol()
{
 p_map_users = new smap<user*,string>(HMAPOCC);
#ifdef NCURSES
 wrap::NCUR->print( GARBAGE );
#endif
#ifdef SERVMSG
 cout << GARBAGE << endl;
#endif
 pthread_mutex_init( &mut_vec_rooms  , NULL);
}

gcol::~gcol()
{
 remove_garbage();
 pthread_mutex_destroy( &mut_vec_rooms );
 delete p_map_users;
}

void
gcol::add_room_to_garbage( room* p_room )
{ 
 pthread_mutex_lock  ( &mut_vec_rooms ); 
 vec_rooms.push_back( p_room ); 
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
 p_user->s_mess_delete();
 p_map_users->add_elem( p_user, tool::to_lower(p_user->get_name()) ); 
#ifdef NCURSES
 wrap::NCUR->print( GARUSER + p_user->get_name() );
#endif
#ifdef SERVMSG
 cout << GARUSER + p_user->get_name() << endl;
#endif
}

bool
gcol::remove_garbage()
{
 bool b_empty;

 pthread_mutex_lock  ( &mut_vec_rooms );
 b_empty  = ( vec_rooms.empty() && p_map_users->get_size() == 0);
 pthread_mutex_unlock( &mut_vec_rooms );

 if ( b_empty )
  return false;

#ifdef NCURSES
 wrap::NCUR->print( GARBACT );
#endif
#ifdef SERVMSG
 cout << GARBACT << endl;
#endif

 pthread_mutex_lock  ( &mut_vec_rooms );
 for ( vector<room*>::iterator iter = vec_rooms.begin();
       iter != vec_rooms.end(); iter++ )
  delete *iter;
 vec_rooms.clear();
 pthread_mutex_unlock( &mut_vec_rooms );


 p_map_users->run_func( delete_users_ );
 p_map_users->make_empty();

 return true;
}

room*
gcol::get_room_from_garbage()
{
 pthread_mutex_lock  ( &mut_vec_rooms );

 if ( vec_rooms.empty() )
 {
  pthread_mutex_unlock( &mut_vec_rooms );
  return NULL;
 }

 room* p_room = vec_rooms.back();
 vec_rooms.pop_back();
 pthread_mutex_unlock( &mut_vec_rooms );

 return p_room;
}

user*
gcol::get_user_from_garbage( string s_user )
{

  user* p_user = p_map_users->get_elem( tool::to_lower(s_user) );;

  if ( p_user != NULL ) 
  {
   p_map_users->del_elem( tool::to_lower(s_user) );
   p_user->set_name( s_user );
   p_user->set_online( true );
#ifdef NCURSES
   wrap::NCUR->print( GARUSE2 + p_user->get_name() );
#endif
#ifdef SERVMSG
   pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
   cout << GARUSE2 << p_user->get_name();
   pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  }


 return p_user;
}

void
gcol::delete_users_( user *user_obj )
{
 delete user_obj;
}

#endif