#ifndef DATA_CPP
#define DATA_CPP

#include <mysql/mysql.h>
#include "data.h"

using namespace std;

data::data( )
{
 vector<string> vec_keys = *wrap::CONF->get_key_vector();

 vector<string>::iterator iter;

 // Reads all DATA_ elements of conf.txt!
 for ( iter = vec_keys.begin(); iter != vec_keys.end(); iter++ )
 {
  if ( (*iter).compare(0, 5, "DATA_") == 0 ) 
  {
   vector<string> vec_tokens;
   string s_element = wrap::CONF->get_elem(*iter); 
   auto unsigned int i_pos;  
  
   for ( bool b_find = 1; b_find; ) 
   {
    i_pos = s_element.find_first_of( " ", 0 );   

    if ( i_pos != string::npos )
    {
     vec_tokens.push_back( s_element.substr(0, i_pos) );
     s_element = s_element.substr( i_pos+1 );
    }

    else
    {
     vec_tokens.push_back( s_element );
     b_find = 0;
    }
   }

   map_queries[*iter] = vec_tokens;
  }
 }
}

data::~data()
{
}

map_string
data::select_user_data( string s_user, string s_query)
{
 return select_user_data( s_user, map_queries[s_query] );
}


map_string
data::select_user_data( string s_user, vector<string> vec_query)
{
 map_string result_map;

 MYSQL* p_mysql = wrap::MMAN->get_connection(); 

 if ( p_mysql == NULL )
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO4 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO4 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  return result_map;
 }

 string s_query = "SELECT ";

 vector<string>::iterator iter = vec_query.begin();

 if ( iter == vec_query.end() )
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO5 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO5 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  return result_map;
 }


 string s_table = *iter; iter++;

 while ( iter != vec_query.end() )
 {
  s_query.append( *iter );
  if ( ++iter != vec_query.end() )
   s_query.append( ", " );
 }

 s_query.append(" FROM " + s_table + " WHERE nick = \"" + s_user + "\"");
 MYSQL_RES* p_result = mysql_query_store_results( p_mysql, s_query );

 if ( p_result != NULL )
 { 
   MYSQL_ROW row;
   iter = vec_query.begin(); iter++;

   while((row = mysql_fetch_row(p_result)))
    for ( int i=0; i < mysql_num_fields(p_result); i++, iter++) 
      result_map[*iter] = row[i];
 }

 mysql_free_result( p_result );
 wrap::MMAN->free_connection( p_mysql );

 return result_map;
}

void
data::insert_user_data( string s_user, string s_query, map_string insert_map )
{
 insert_user_data( s_user, map_queries[s_query], insert_map );
}

void
data::insert_user_data( string s_user, vector<string> vec_query, map_string insert_map )
{
 MYSQL* p_mysql = wrap::MMAN->get_connection(); 

 if ( p_mysql == NULL )
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO4 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO4 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  return;
 }

 vector<string>::iterator iter = vec_query.begin();

 if ( iter == vec_query.end() )
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO5 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO5 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  return;
 }

 string s_table = *iter; iter++;

 string s_query = "INSERT INTO " + s_table + " (";

 while ( iter != vec_query.end() )
 {
  s_query.append( *iter );
  if ( ++iter != vec_query.end() )
   s_query.append( ", " );
  else
   s_query.append( ") VALUES(" );
 }
 iter = vec_query.begin(); iter++;

 while ( iter != vec_query.end() )
 {
  s_query.append( "\"" + insert_map[*iter] + "\"" );
  if ( ++iter != vec_query.end() )
   s_query.append( ", " );
  else
   s_query.append( ") " );
 }

 MYSQL_RES* p_result = mysql_query_store_results( p_mysql, s_query );
 mysql_free_result( p_result );
 wrap::MMAN->free_connection( p_mysql );
}


void
data::update_user_data( string s_user, string s_query, map_string update_map )
{
 update_user_data( s_user, map_queries[s_query], update_map );
}

void
data::update_user_data( string s_user, vector<string> vec_query, map_string update_map )
{
 if ( update_map.size() == 0 )
  return;

 MYSQL* p_mysql = wrap::MMAN->get_connection(); 

 if ( p_mysql == NULL )
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO4 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO4 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  return;
 }

 vector<string>::iterator iter = vec_query.begin();
 vector<string>::iterator iter_second = vec_query.begin();
 iter_second++;

 if ( iter == vec_query.end() )
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO5 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO5 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
  return;
 }

 string s_table = *iter; iter++;
 string s_query = "UPDATE " + s_table + " SET ";
 bool b_flag = 0;

 while ( iter != vec_query.end() )
 {
  if ( update_map[*iter] == "" ) // Dont update data if it has not been changed!
  {
   iter++; 
   continue;
  } 
  if ( iter != iter_second && b_flag )
   s_query.append( ", " );
  s_query.append( *iter + "=\"" + update_map[*iter] + "\"" );
  b_flag = 1;
  iter++;
 }

 s_query.append( " WHERE nick=\"" + tool::to_lower(s_user) + "\"" );
 MYSQL_RES* p_result = mysql_query_store_results( p_mysql, s_query );
 mysql_free_result( p_result );
 wrap::MMAN->free_connection( p_mysql );
}


MYSQL_RES*
data::mysql_query_store_results( MYSQL* p_mysql, string s_query )
{
#ifdef QUERIES
 print_query( s_query );
#endif
 
 int i_status = mysql_query( p_mysql, s_query.c_str() );

 if ( i_status == 0 ) 
 {
  return mysql_store_result( p_mysql );
 }

 else
 {
#ifdef NCURSES
  wrap::NCUR->print( MYERRO4 );
#endif
#ifdef SERVMSG
  pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
  cerr << MYERRO4 << endl;
  pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
 }
 return NULL;
}



#ifdef QUERIES
void
data::print_query( string s_query )
{
#ifdef NCURSES
 wrap::NCUR->print( MYMANAG + s_query );
#endif
#ifdef SERVMSG
 pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
 cout << MYMANAG + s_query << endl;
 pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
}
#endif

#endif
