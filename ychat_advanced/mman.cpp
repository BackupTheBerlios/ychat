#ifndef MMAN_CPP
#define MMAN_CPP

#include "mman.h"
#include "mcon.h"
#include "tool.h"

using namespace std;

mman::mman()
{
    int i_initial = tool::string2int( wrap::CONF->get_val( "MIN_CONNECTIONS" ) );
    int i_max     = tool::string2int( wrap::CONF->get_val( "MAX_CONNECTIONS" ) );

    pthread_mutex_init( &mut_i_used_con  , NULL);
    pthread_mutex_init( &mut_vec_mysql  , NULL);

    this->i_used_connections=0;

    if( i_max > MAXMSQL )
    {
#ifdef VERBOSE
    cerr << MYLIMIT << MAXMSQL << endl;
#endif
#ifdef NCURSES
    wrap::NCUR->print( string(MYLIMIT) + tool::int2string(MAXMSQL) );
#endif

     i_max = MAXMSQL;
    }

    if( i_initial > i_max )
        i_initial = i_max;

    this->i_initial_connections = i_initial;
    this->i_max_connections = i_max;

#ifdef VERBOSE
    cout << MYINITC << i_initial << endl
         << MYINITM << i_max << endl;
#endif

 init( wrap::CONF->get_val( "MYSQL_HOST" ),
       wrap::CONF->get_val( "MYSQL_USER" ), 
       wrap::CONF->get_val( "MYSQL_PASS" ), 
       wrap::CONF->get_val( "MYSQL_DB" ), 
       tool::string2int( wrap::CONF->get_val( "MYSQL_PORT" ) ) ); 
}
void mman::init( string host, string user, string passwd, string db, unsigned int port)
{
    this->s_host=host;
    this->s_user=user;
    this->s_pass=passwd;
    this->s_db=db;
    this->i_port=port;

    pthread_mutex_lock  ( &mut_vec_mysql );
    for(int i=0; i<this->i_initial_connections;i++)
        mysql.push_back( new_connection( ) );
    pthread_mutex_unlock( &mut_vec_mysql );
}

#ifdef NCURSES
void
mman::print_init_ncurses()
{
        string s_tmp( MYINITC );
        s_tmp.append( tool::int2string( i_initial_connections ) );
        wrap::NCUR->print( s_tmp );

        string s_tmp2( MYINITM );
        s_tmp2.append( tool::int2string( i_max_connections ) );
        wrap::NCUR->print( s_tmp2 );
        print_used_connections(1);
}
void
mman::print_used_connections( bool b_refresh )
{
    pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
    mvprintw( NCUR_MYSQL_X,NCUR_MYSQL_Y, "MySQL: %d ", i_used_connections);
    if ( b_refresh )
     refresh();
    pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
}
#endif

mman::~mman()
{
    pthread_mutex_lock  ( &mut_vec_mysql );
    for( int i = 0; i < mysql.size();i++)
    {
        if( mysql[i] != NULL )
            mysql_close( mysql[i] );
    }

    pthread_mutex_unlock( &mut_vec_mysql );
    pthread_mutex_destroy( &mut_i_used_con );
    pthread_mutex_destroy( &mut_vec_mysql );
}

MYSQL *mman::get_connection()
{
    pthread_mutex_lock  ( &mut_vec_mysql );
    if( ! mysql.empty() )
    {
        MYSQL *x = mysql[mysql.size()-1];
        mysql.pop_back();

        if(mysql_ping(x) == 0)
        {
            pthread_mutex_lock  ( &mut_i_used_con );
            this->i_used_connections--;
#ifdef NCURSES
            print_used_connections(1);
#endif
            pthread_mutex_unlock( &mut_i_used_con );
            mysql_close(x);

            pthread_mutex_unlock( &mut_vec_mysql );
            return new_connection();
        }
        pthread_mutex_unlock( &mut_vec_mysql );
        return x;
    }
    pthread_mutex_unlock( &mut_vec_mysql );
 
#ifdef NCURSES
    wrap::NCUR->print( MYERROR );
#endif
#ifdef SERVMSG
    cerr << MYERROR << endl;
#endif

    return NULL;
}
MYSQL *mman::new_connection( )
{
    pthread_mutex_lock  ( &mut_i_used_con );
    if( i_used_connections > i_max_connections )
    {
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

    MYSQL *ms = mysql_init(NULL);

    if( ! ms )
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

    if( mysql_real_connect( ms, s_host.c_str(), 
                                s_user.c_str(), 
                                s_pass.c_str(), 
                                s_db.c_str(),
                                i_port, 
                                NULL, 0 ) == NULL )
    {
#ifdef NCURSES
	//wrap::NCUR->shutdown();
        //wrap::NCUR->print( string( MYMANAG ) + string(  mysql_error(ms) ) );
        cerr << MYMANAG << mysql_error(ms) << endl;
#endif
#ifdef SERVMSG
        cerr << MYMANAG << mysql_error(ms) << endl;
#endif
        return NULL; 
    }

    pthread_mutex_lock  ( &mut_i_used_con );
    i_used_connections++;
#ifdef NCURSES
    print_used_connections(0);
#endif
    pthread_mutex_unlock( &mut_i_used_con );
    return ms;
}
void mman::free_connection( MYSQL *msql)
{
    pthread_mutex_lock  ( &mut_i_used_con );
    i_used_connections--;
#ifdef NCURSES
    print_used_connections(1);
#endif
    pthread_mutex_unlock( &mut_i_used_con );

    if(mysql_ping( msql )==0)
    {
        mysql_close( msql );
        pthread_mutex_lock  ( &mut_vec_mysql );
        mysql.push_back( msql );
        pthread_mutex_unlock( &mut_vec_mysql );
    }

    else
    {
        pthread_mutex_lock  ( &mut_vec_mysql );
        mysql.push_back( new_connection() );
        pthread_mutex_unlock( &mut_vec_mysql );
    }
}

#endif
