#ifndef TIMR_CPP
#define TIMR_CPP

#include <sys/time.h>
#include "timr.h"

using namespace std;

timr::timr()
{
  b_timer_active = true;
  pthread_mutex_init( &mut_s_time, NULL);
  pthread_mutex_init( &mut_s_uptime, NULL);
  s_time = "00:00:00";
  s_uptime = "00:00:00";
}

timr::~timr()
{
  pthread_mutex_destroy( &mut_s_time );
  pthread_mutex_destroy( &mut_s_uptime );
}

bool
timr::get_timer_active() const
{
 return b_timer_active;
}

void*
timr::start( void *v_pointer )
{
    timr* p_timer = static_cast<timr*>(v_pointer);

#ifdef NCURSES
    wrap::NCUR->print( TIMERAC );
#endif
#ifdef SERVMSG
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    cout << TIMERAC << endl;
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
#ifdef NCURSES
    p_timer->print_time( ); 
#endif
    time_t clock_start;
    time_t clock_now;

    time( &clock_start );
    tm time_start = *localtime( &clock_start );
    tm time_now;
    while ( p_timer->get_timer_active() )
    {
     // sleep a second!
     usleep( 1000000 );

     // get the current time!
     time( &clock_now );

     time_now = *localtime( &clock_now );

     // set the current time && the current ychat uptime!
     p_timer->set_time( difftime( clock_now, clock_start ),
                        time_now.tm_sec, time_now.tm_min, time_now.tm_hour );
#ifdef NCURSES
     p_timer->print_time( );
#endif      
     // run every minute:
     if ( time_now.tm_sec == 0 )
     { 
#ifdef SERVMSG
      pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
      cout << TIMERUP << p_timer->get_uptime() << endl;
      pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
      wrap::CHAT->msg_post( new string( "<!-- PING! //-->\n" ) );
    
      // run every ten minutes: 
      if ( time_now.tm_min % 10 == 0 )
       wrap::GCOL->remove_garbage();
     }
    }

}

#ifdef NCURSES
void
timr::print_time( )
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    mvprintw( NCUR_TIME_X, NCUR_TIME_Y, "Time:   %s ", get_time().c_str());
    mvprintw( NCUR_UPTIME_X, NCUR_UPTIME_Y, "Uptime: %s ", get_uptime().c_str());
    refresh();
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
}
#endif

void
timr::set_time( double d_uptime, int i_cur_seconds, int i_cur_minutes, int i_cur_hours )
{
    int i_hours = (int) d_uptime / 3600; 
    int i_minutes = (int) d_uptime / 60; 
    while ( i_minutes >= 60 )
     i_minutes -= 60;

    while ( d_uptime >= 60 )
     d_uptime -= 60;

    pthread_mutex_lock  ( &mut_s_time );
    s_time = add_zero_to_front( tool::int2string( i_cur_hours ) ) + ":" +
             add_zero_to_front( tool::int2string( i_cur_minutes ) ) + ":" +
             add_zero_to_front( tool::int2string( i_cur_seconds ) );
    pthread_mutex_unlock( &mut_s_time );

    pthread_mutex_lock  ( &mut_s_uptime );
    s_uptime = add_zero_to_front( tool::int2string( i_hours ) ) + ":" +
               add_zero_to_front( tool::int2string( i_minutes ) )  + ":" +
               add_zero_to_front( tool::int2string( (int) d_uptime ) );
    pthread_mutex_unlock( &mut_s_uptime );
}

string
timr::add_zero_to_front( string s_time )
{
    if ( s_time.length() == 1 )
    {
     string s_new = "0" + s_time;
     return s_new;
    }

    return s_time;
}
#endif