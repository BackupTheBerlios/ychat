
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

 pthread_mutex_t mut_s_uptime;
 
public:
    timr();
    ~timr();

    bool get_timer_active() const;
    static void* start( void *v_pointer );

#ifdef NCURSES
    void print_uptime( );
#endif

    void set_uptime( int i_minutes, int i_hours );
    string get_uptime( );
    string* add_zero_to_front( string *s_time );
};

#endif
