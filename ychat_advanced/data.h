#include "incl.h"

#ifndef DATA_H
#define DATA_H

#include <mysql/mysql.h>

using namespace std;


class data
{
 private:
 map< string,vector<string> > map_queries;

 MYSQL_RES* mysql_query_store_results( MYSQL* p_mysql, string s_query );
 virtual map_string select_user_data( string s_user, vector<string> vec_query );
 virtual void       insert_user_data( string s_user, vector<string> vec_query, map_string insert_map );
 virtual void       update_user_data( string s_user, vector<string> vec_query, map_string update_map );


#ifdef QUERIES
 void print_query( string s_query );
#endif

 public:
  data();
  ~data();

 virtual map_string select_user_data( string s_user, string s_query );
 virtual void       insert_user_data( string s_user, string s_query, map_string insert_map );
 virtual void       update_user_data( string s_user, string s_query, map_string update_map );
};

#endif
