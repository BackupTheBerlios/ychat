#include "incl.h"

#ifndef MMAN_H
#define MMAN_H

#include <mysql/mysql.h>
#include <vector>
#include "mcon.h"

using namespace std;
class mman
{
private:
    vector<MYSQL*> mysql;
    string s_host;
    string s_user;
    string s_pass;
    string s_db;

    unsigned int i_port;
    int i_initial_connections;
    int i_max_connections;
    int i_active_connections;

    pthread_mutex_t mut_i_used_con;
    pthread_mutex_t mut_vec_mysql;
    MYSQL* new_connection( );

public:
    mman();
    ~mman();
    void init( string host, string user, string passwd, string db, unsigned int port = PRTMSQL );
    MYSQL* get_connection();
    void free_connection( MYSQL *p_mysql_con );
#ifdef NCURSES
    void print_active_connections( bool b_refresh );
#endif	

};
#endif
