// class room declaration.

#include "incl.h"

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

public:
    void add_user( user* p_user  )
    {
        p_user->set_p_room( this );
        add_elem( p_user );
    }

    user* get_user( string &s_name, bool &b_found )
    {
        return static_cast<user*>( get_elem( s_name, b_found ) );
    }

    explicit room( string s_name );      // a constructor.
    ~room();  			       // room destructor.

    virtual string get_topic();
    virtual void set_topic( string s_topic );
};

#endif
