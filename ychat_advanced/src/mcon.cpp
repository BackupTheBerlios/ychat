#ifndef MCON_CPP
#define MCON_CPP

#include "mcon.h"

mcon::mcon( MYSQL *mysql )
{
    this->query_sent = false;

    if( mysql == NULL )
    {
        wrap::system_message( MYWARN1 );
    }

    this->myc = mysql;
}
mcon::~mcon()
{
    if(this->query_sent)
        mysql_free_result(this->current_result);
}
int mcon::query( string query )
{
    mysql_real_query( this->myc, query.c_str(), query.size() );

    if(mysql_error(this->myc)[0] != '\0')
    {
        string s_error( mysql_error( this->myc ) );
        wrap::system_message( MYERRO3 + s_error );
        return -1;
    }
    this->query_sent = true;

    this->current_result = mysql_store_result( this->myc );
}
MYSQL_ROW mcon::get_next_row()
{
    if( query_sent )
    {
        return mysql_fetch_row( this->current_result );
    }

    return NULL;
}
#endif
