#ifndef MMAN_CXX
#define MMAN_CXX

#include "mman.h"
#include "mcon.h"

using namespace std;

mman::mman(int initial, int max) {

	this->i_used_connections=0;	

	if(max>CONNECTIONS_MAX)
	{
		cerr << LOG_PREFIX << "reached hardlimit of mysql connections! using " << CONNECTIONS_MAX << " as connection limit" << endl;
		max=CONNECTIONS_MAX;
	}
	
	if(initial>max)
		initial=max;
	this->i_initial_connections=initial;	
	this->i_max_connections=max;
#ifdef VERBOSE
	cout << LOG_PREFIX << "initial connections: " << initial << endl;
	cout << LOG_PREFIX << "max. connections: " << max << endl;	
#endif
}
void mman::init( string host, string user, string passwd, string db, unsigned int port)
{
	this->s_host=host;
	this->s_user=user;
	this->s_pass=passwd;
	this->s_db=db;
	this->i_port=port;

	 for(int i=0; i<this->i_initial_connections;i++)
                this->mysql.push_back(new_connection( ));

}
mman::~mman() {

	for(int i = 0; i < this->mysql.size();i++)
	{
		if(this->mysql[i] != NULL)
			mysql_close(this->mysql[i]);
	}
}
MYSQL *mman::get_connection(){

	if(!this->mysql.empty())
	{
		MYSQL *x=this->mysql[this->mysql.size()-1];
		this->mysql.pop_back();
		if(mysql_ping(x)==0)
		{
			this->i_used_connections--;
			mysql_close(x);
			return new_connection();
		}
		return x;
	}
	cerr << LOG_PREFIX << " no suitable connection found " << endl;
	return NULL;
}
MYSQL *mman::new_connection( ){
	
	
	if(this->i_used_connections>this->i_max_connections)
	{
		cerr << LOG_PREFIX << "reached connection limit" << endl;
		return NULL;
	}
	MYSQL *ms = mysql_init(NULL);
	if(!ms)
	{
		cerr << LOG_PREFIX << "init failed" << endl;
		exit(1);
	}	
	if(mysql_real_connect( ms, this->s_host.c_str(), this->s_user.c_str(), this->s_pass.c_str(), this->s_db.c_str()
				 , this->i_port, NULL, 0 )==NULL)
        {
		cerr << LOG_PREFIX << mysql_error(ms) << endl;
		exit(1);	
	}
	this->i_used_connections++;
	return ms;
}
void mman::free_connection( MYSQL *msql){
	this->i_used_connections--;
	if(mysql_ping( msql )==0)
	{
		mysql_close( msql );
		this->mysql.push_back( msql );
	}
	else
		this->mysql.push_back( new_connection() );
}
#endif
