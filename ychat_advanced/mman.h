#ifndef MMAN_H
#define MMAN_H
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include "mcon.h"

#define CONNECTIONS_MAX 20
#define LOG_PREFIX "MySQL Connection Manager: " 
using namespace std;
class mman {
	
	
	public:
		mman(int initial, int max);
		~mman();
		void init( string host, string user, string passwd, string db, unsigned int port=3306 );
		MYSQL *get_connection();
		MYSQL *new_connection( );
		void free_connection( MYSQL *msql );
	private:
		vector<MYSQL *> mysql;
		string s_host;
		string s_user;
		string s_pass;
		string s_db;
		unsigned int i_port;
		int i_initial_connections;
		int i_max_connections;
		int i_used_connections;

};
#endif
