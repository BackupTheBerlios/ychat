
#ifndef TIMR_H
#define TIMR_H

#include <unistd.h>
#include "incl.h"

using namespace std;

class timr
{
private:
 bool b_timer_active;
 string s_uptime;
 string s_time;

 pthread_mutex_t mut_s_time;
 pthread_mutex_t mut_s_uptime;
 
public:
    timr();
    ~timr();

    bool get_timer_active() const;
    static void* start( void *v_pointer );

#ifdef NCURSES
    void print_time( );
#endif

    void set_time( double d_uptime, int i_cur_seconds, int i_cur_minutes, int i_cur_hours );
    string get_uptime( );
    string get_time( );
    string add_zero_to_front( string s_time );
};

#endif
