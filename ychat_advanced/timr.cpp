#ifndef TIMR_CPP
#define TIMR_CPP

#include "timr.h"
#include "s_ncur.h"
#include "s_mutx.h"
#include "s_tool.h"

using namespace std;

timr::timr()
{
  b_timer_active = true;
  pthread_mutex_init( &mut_s_uptime, NULL);
  s_uptime = "00:00";
}

timr::~timr()
{
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
    s_ncur::get().print( TIMERAC );
#endif
#ifdef SERVMSG
    pthread_mutex_lock  ( &s_mutx::get
                              ().mut_stdout );
    cout << TIMERAC << endl;
    pthread_mutex_unlock( &s_mutx::get
                              ().mut_stdout );
#endif
    int i_seconds, i_minutes, i_hours;
    i_seconds = i_minutes = i_hours = 0;

#ifdef NCURSES
    p_timer->print_uptime( ); 
#endif


    while ( p_timer->get_timer_active() )
    {
     // sleep a second!
     usleep( 1000000 );
     i_seconds++;

     if ( i_seconds == 60 )
     {
      i_seconds = 0;
      i_minutes++;

      if ( i_minutes == 60 )
      { 
       i_minutes = 0;
       i_hours++;
      }

      p_timer->set_uptime( i_minutes, i_hours );

#ifdef NCURSES
      p_timer->print_uptime( );
#endif      

#ifdef SERVMSG
      pthread_mutex_lock  ( &s_mutx::get
                              ().mut_stdout );
      cout << TIMERUP << p_timer->get_uptime() << endl;
      pthread_mutex_unlock( &s_mutx::get
                              ().mut_stdout );
#endif
     }
    }
}

#ifdef NCURSES
void
timr::print_uptime( )
{
    s_uptime.append( "h" );
    pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
    mvprintw( NCUR_UPTIME_X, NCUR_UPTIME_Y, "Uptime: %s ", s_uptime.c_str());
    refresh();
    pthread_mutex_unlock( &s_mutx::get().mut_stdout );
}
#endif

void
timr::set_uptime( int i_minutes, int i_hours )
{
    pthread_mutex_lock  ( &mut_s_uptime );
    s_uptime = *add_zero_to_front( new string( s_tool::int2string( i_hours ) ) ) + ":" +
               *add_zero_to_front( new string( s_tool::int2string( i_minutes ) ) );
    pthread_mutex_unlock( &mut_s_uptime );
}

string
timr::get_uptime(  )
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_uptime );
    s_ret = this->s_uptime;
    pthread_mutex_unlock( &mut_s_uptime );
    return s_ret;
}

string*
timr::add_zero_to_front( string* s_time )
{
    if ( s_time->length() == 1 )
    {
     string *s_new = new string( "0" );
     s_new->append( *s_time );
     return s_new;
    }
    return s_time;
}
#endif
