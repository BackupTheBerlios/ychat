#include "incl.h"

#ifndef USER_H
#define USER_H

#include "smap.h"
#include "name.h"

class room;

using namespace std;

class user : public name
{
private:
    // private members:
    bool   b_online; // true if user is online.
    bool   b_has_sess; // true if user already has a session! 
    bool   b_is_reg; // true if user is registered 
    bool   b_away;   // true if user is away.
    long   l_time;   // user's last activity time.
    rang   r_rang;   // user's rang ( see enum rang @ globals.h ).
    rang   r_oldr;   // user's previous rang.
    string s_tmpid;
    string s_agnt;   // user's http user agent.
    string s_away;   // user's last away message.
    string s_col1;   // user's nick color.
    string s_col2;   // user's text color.
    string s_email;  // user's email addres 
    string s_mess;   // message string which has to be sent to the user.
    string s_pass;   // password
    room*  p_room;   // pointer to the user's room.

    map_string map_changed_data; // Needed to tell yChat which data to change after user is removed!
    bool             b_set_changed_data; // Only set change data if required! 

    // Modules which are allowed to be executed by the user.
    smap<dynmod*,string>* map_mods;

    pthread_mutex_t mut_map_mods;
    pthread_mutex_t mut_away;
    pthread_mutex_t mut_b_online;
    pthread_mutex_t mut_b_has_sess;
    pthread_mutex_t mut_b_is_reg;
    pthread_mutex_t mut_l_time;
    pthread_mutex_t mut_s_mess;
    pthread_mutex_t mut_s_pass;
    pthread_mutex_t mut_p_room;
    pthread_mutex_t mut_s_col1;
    pthread_mutex_t mut_s_col2;
    pthread_mutex_t mut_s_email;
    pthread_mutex_t mut_s_tmpid;
    pthread_mutex_t mut_r_rang;
    pthread_mutex_t mut_map_changed_data;

    void initialize();
    virtual void   set_changed_data( string s_varname, string s_value );

public:
    pthread_cond_t  cond_message;
    pthread_mutex_t mut_message;

    // public methods:
    explicit user();
    explicit user( string s_name );      // a standard constructor.
    ~user();			      // user destructor.

    // gets specific data of this user und stores it in
    // (*p_map_string)["nick"]. this method will be used
    // every time data has to be got from every user of a room
    // or even of the system.
    virtual void get_data( map_string *p_map_data );

    virtual string get_colored_name();
    string get_colored_bold_name()
    { 
     return "<b>" + get_colored_name() + "</b>";
    }
    virtual bool  get_online();
    virtual bool  get_has_sess();
    virtual bool  get_is_reg();
    virtual void  set_online( bool b_online );
    virtual void  set_has_sess( bool b_has_sess );
    virtual void  set_is_reg( bool b_is_reg );
    void  set_changed_data_on() { b_set_changed_data = 1; }
    virtual bool  get_away( );
    virtual string  get_away_msg( );
    virtual void  set_away( bool b_away, string s_away );
    virtual void  set_away( bool b_away );
    virtual room* get_room();
    virtual void  set_p_room( room* p_room );
    virtual string get_pass();
    virtual string get_col1();
    virtual string get_col2();
    virtual string get_email();
    virtual string get_tmpid();
    virtual void   set_tmpid( string s_tmpid );
    virtual void   set_pass( string s_col1 );
    virtual void   set_col1( string s_col1 );
    virtual void   set_col2( string s_col2 );
    virtual void   set_email( string s_email );
    virtual rang   get_rang( );
    virtual void   set_rang( rang   r_rang );
    virtual bool   new_msgs  ( );

    // executes a command.
    virtual void command( string &s_command );

    // gets the message and clears s_mess;
    virtual string get_mess();

    // actualizes the user's timestamp.
    virtual void  renew_stamp();

    // Here are starting methods which are mainly needed by the data<type> class.
    // appends a string to s_mess including br.
    virtual void msg_post( string *p_msg );
    virtual void s_mess_delete( );
    virtual void get_user_list( string &s_list, string &s_seperator );
    virtual void check_restore_away();
};

#endif
