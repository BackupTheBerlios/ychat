#ifndef MMAN_CPP
#define MMAN_CPP

#include "mman.h"
#include "mcon.h"
#include "tool.h"

using namespace std;

mman::mman()
{
#ifdef NCURSES
     wrap::NCUR->print( MYSINIT  );
#endif
#ifdef VERBOSE
     cout << MYSINIT << endl;
#endif

    int i_initial = tool::string2int( wrap::CONF->get_elem( "MYSQL_MIN_CON" ) );
    int i_max     = tool::string2int( wrap::CONF->get_elem( "MYSQL_MAX_CON" ) );

    pthread_mutex_init( &mut_i_used_con  , NULL);
    pthread_mutex_init( &mut_vec_mysql  , NULL);

    i_active_connections = 0;

    if( i_max > MAXMSQL )
    {
#ifdef NCURSES
     wrap::NCUR->print( MYLIMIT + tool::int2string(MAXMSQL) );
#endif
#ifdef VERBOSE
     cerr << MYLIMIT << MAXMSQL << endl;
#endif
     i_max = MAXMSQL;
    }

    if( i_initial > i_max )
        i_initial = i_max;

    i_initial_connections = i_initial;
    i_max_connections = i_max;

#ifdef NCURSES
    wrap::NCUR->print(  MYINITC + tool::int2string( i_initial ) );
    wrap::NCUR->print(  MYINITM + tool::int2string( i_max ) );
#endif
#ifdef SERVMSG
    cout << MYINITC << i_initial << endl
         << MYINITM << i_max << endl;
#endif

 init( wrap::CONF->get_elem( "MYSQL_HOST" ),
       wrap::CONF->get_elem( "MYSQL_USER" ), 
       wrap::CONF->get_elem( "MYSQL_PASS" ), 
       wrap::CONF->get_elem( "MYSQL_DB" ), 
       tool::string2int( wrap::CONF->get_elem( "MYSQL_PORT" ) ) ); 

#ifdef NCURSES
 print_active_connections( 1 );
#endif
}

mman::~mman()
{
    pthread_mutex_lock  ( &mut_vec_mysql );
    for( vector<MYSQL*>::iterator iter = mysql.begin(); 
         iter != mysql.end(); 
         iter++)
    {
          if( *iter != NULL )
            mysql_close( *iter );
    }
    mysql.clear();
    pthread_mutex_unlock( &mut_vec_mysql );

    pthread_mutex_destroy( &mut_i_used_con );
    pthread_mutex_destroy( &mut_vec_mysql );
}

void
mman::init( string host, string user, string passwd, string db, unsigned int port)
{
    s_host = host;
    s_user = user;
    s_pass = passwd;
    s_db = db;
    i_port = port;

    pthread_mutex_lock  ( &mut_vec_mysql );

    for (int i = 0; i < i_initial_connections; i++)
        mysql.push_back( new_connection() );

    pthread_mutex_unlock( &mut_vec_mysql );
}

MYSQL*
mman::get_connection()
{
    pthread_mutex_lock  ( &mut_vec_mysql );
    if( ! mysql.empty() )
    {
        MYSQL* p_mysql_con = mysql.back();
        mysql.pop_back();
        pthread_mutex_unlock( &mut_vec_mysql );

        if ( mysql_ping(p_mysql_con) != 0 )
        {
            mysql_close(p_mysql_con);
            return new_connection();
        }
        return p_mysql_con;
    }
    return new_connection();
}

MYSQL*
mman::new_connection( )
{
    pthread_mutex_lock  ( &mut_i_used_con );
    i_active_connections++;
#ifdef NCURSES
    wrap::NCUR->print( MYSCONN + tool::int2string(i_active_connections) );
    print_active_connections(1);
#endif
#ifdef SERVMSG
    cout << MYSCONN << i_active_connections << endl;
#endif
    pthread_mutex_unlock( &mut_i_used_con );

    pthread_mutex_lock  ( &mut_i_used_con );
    if( i_active_connections >= i_max_connections )
    {
     pthread_mutex_unlock( &mut_i_used_con );
#ifdef NCURSES
	wrap::NCUR->shutdown();
    	cerr << MYERRO2 << endl;
#endif
#ifdef SERVMSG
    	cerr << MYERRO2 << endl;
#endif
        return NULL;
    }
    pthread_mutex_unlock( &mut_i_used_con );

    MYSQL* p_mysql_con = mysql_init(NULL);

    if (!p_mysql_con)
    {
#ifdef NCURSES
	wrap::NCUR->shutdown();
    	cerr << MYERRO1 << endl;
#endif
#ifdef SERVMSG
    	cerr << MYERRO1 << endl;
#endif
        return NULL; 
    }

    if( mysql_real_connect( p_mysql_con, s_host.c_str(), 
                                s_user.c_str(), 
                                s_pass.c_str(), 
                                s_db.c_str(),
                                i_port, 
                                NULL, 0 ) == NULL )
    {
#ifdef NCURSES
        wrap::NCUR->print( MYMANAG + string(  mysql_error(p_mysql_con) ) );
#endif
#ifdef SERVMSG
        cerr << MYMANAG << mysql_error(p_mysql_con) << endl;
#endif
        return NULL; 
    }

    return p_mysql_con;
}

void
mman::free_connection( MYSQL *p_mysql_con )
{
    if (mysql_ping( p_mysql_con ) != 0)
    {
        mysql_close( p_mysql_con );
        pthread_mutex_lock  ( &mut_vec_mysql );
        mysql.push_back( new_connection() );
        pthread_mutex_unlock( &mut_vec_mysql );
    }

    else
    {
        pthread_mutex_lock  ( &mut_vec_mysql );
        mysql.push_back( p_mysql_con );
        pthread_mutex_unlock( &mut_vec_mysql );
    }
}

#ifdef NCURSES
void
mman::print_active_connections( bool b_refresh )
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    mvprintw( NCUR_MYSQL_X,NCUR_MYSQL_Y, "MySQL: %d ", i_active_connections);
    if ( b_refresh )
     refresh();
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
}
#endif

#endif
