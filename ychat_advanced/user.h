// class user declaration.
#ifndef USER_H
#define USER_H

#include "incl.h"
#include "hmap.h"
#include "name.h"
#include "s_lang.h"
using namespace std;

class room;

class user : public name
{
private:
    // private members:
    bool   b_away;   // true if user is away.
    bool   b_online; // true if user is online.
    int    i_sock;   // user's stream socket descriptor.
    long   l_time;   // user's last activity time.
    rang   r_rang;   // user's rang ( see enum rang @ globals.h ).
    rang   r_oldr;   // user's previous rang.
    string s_id;
    string s_agnt;   // user's http user agent.
    string s_away;   // user's last away message.
    string s_col1;   // user's nick color.
    string s_mess;   // message string which has to be sent to the user.
    room*  p_room;   // pointer to the user's room.

    // Modules which are allowed to be executed by the user.
    hmap<dynmod*,string>* map_mods;

    pthread_mutex_t mut_map_mods;
    pthread_mutex_t mut_b_online;
    pthread_mutex_t mut_i_sock;
    pthread_mutex_t mut_l_time;
    pthread_mutex_t mut_s_mess;
    pthread_mutex_t mut_p_room;
    pthread_mutex_t mut_s_col1;
    pthread_mutex_t mut_s_id;
    pthread_mutex_t mut_r_rang;

public:
    pthread_cond_t  cond_message;
    pthread_mutex_t mut_message;

    // small inline methods:
    string get_col1()
    {
	string s_ret;
        pthread_mutex_lock  ( &mut_s_col1 );
        s_ret = s_col1;
        pthread_mutex_unlock( &mut_s_col1 );
        return s_ret;
    }
    string get_id()
    {
	string s_ret;
        pthread_mutex_lock  ( &mut_s_id );
        s_ret = s_id;
        pthread_mutex_unlock( &mut_s_id );
        return s_ret;
    }
    void   set_id    ( string s_id   )
    {
        pthread_mutex_lock  ( &mut_s_id );
        this -> s_id = s_id;
        pthread_mutex_unlock( &mut_s_id );
    }
    void   set_col1  ( string s_col1 )
    {
        pthread_mutex_lock  ( &mut_s_col1 );
        this -> s_col1 = s_col1;
        pthread_mutex_unlock( &mut_s_col1 );
    }

    rang   get_rang  ( )
    {
	rang r_ret;
        pthread_mutex_lock  ( &mut_r_rang );
	r_ret = r_rang;
        pthread_mutex_unlock(&mut_r_rang );
        return  r_ret;
    }
    void   set_rang  ( rang   r_rang )
    {
        pthread_mutex_lock  ( &mut_r_rang );
        r_oldr = this -> r_rang;
        this -> r_rang = r_rang;
        pthread_mutex_unlock( &mut_r_rang );
    }

    bool   new_msgs  ( )
    {
        pthread_mutex_lock  ( &mut_s_mess );
        return s_mess.empty();
        pthread_mutex_unlock( &mut_s_mess );
    }
    // public methods:
    explicit user( string s_name );      // a standard constructor.
    ~user();			      // user destructor.

    // gets specific data of this user und stores it in
    // (*p_map_string)["nick"]. this method will be used
    // every time data has to be got from every user of a room
    // or even of the system.
    virtual void get_data( map_string *p_map_data );

    virtual bool  get_online();
    virtual void  set_online( bool b_online );
    virtual room* get_room();
    virtual void  set_p_room( room* p_room );
    virtual int   get_sock  ( );
    virtual void  set_sock  ( int i_sock );

    // executes a command.
    virtual void command( string &s_command );

    // gets the message and clears s_mess;
    virtual string get_mess();

    // actualizes the user's timestamp.
    virtual void  renew_stamp();

    // Here are starting methods which are mainly needed by the data<type> class.

    // appends a string to s_mess including br.
    virtual void msg_post( string *p_msg );
    virtual void get_user_list( string &s_list, string &s_seperator );
};

#endif
