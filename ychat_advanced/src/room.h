// class room declaration.

#include "incl.h"
#include "wrap.h"

#ifndef ROOM_H
#define ROOM_H

#include "base.h"
#include "name.h"
#include "user.h"
#include "tool.h"

using namespace std;

class room : public base<user>, public name
{
private:
    string s_topic;
    pthread_mutex_t mut_s_topic;
    virtual void reload_onlineframe();

public:
    room( string s_name );
    ~room();  		 

    void add_user( user* p_user  )
    {
        add_elem( p_user );
        reload_onlineframe(); 
        p_user->set_p_room( this );
    }

    user* get_user( string &s_name, bool &b_found )
    {
        return static_cast<user*>( get_elem( s_name, b_found ) );
    }

    void del_elem( string &s_name )
    {
     smap<user*,string>::del_elem( s_name ); 
     if ( smap<user*,string>::get_size() == 0 ) 
      clean_room();
     else
      reload_onlineframe(); 
    }

    virtual string get_topic();
    virtual void set_topic( string s_topic );
    virtual void clean_room();

};

#endif
