// garbage collector 
#include "incl.h"

#ifndef GCOL_H
#define GCOL_H

#include <vector> 
#include "room.h"
#include "user.h"

using namespace std;

class gcol
{
 private:
  vector<room*> p_vec_rooms; 
  vector<user*> p_vec_users; 

  pthread_mutex_t mut_vec_rooms;
  pthread_mutex_t mut_vec_users;

 public:
  gcol();
  ~gcol();

 bool remove_garbage();
 void add_room_to_garbage( room* p_room );
 void add_user_to_garbage( user* p_user );
};

#endif
