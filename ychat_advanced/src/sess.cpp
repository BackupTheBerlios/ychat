#ifndef SESS_CPP
#define SESS_CPP

#include "sess.h"

sess::sess( string s_id )
{
    this->sess_id=s_id;
}

sess::~sess()
{
}

string
sess::get_id()
{
    return this->sess_id;
}


void
sess::invalidate()
{
    this->sess_id = "0";
    this->sess_values.clear();
}

void
sess::set_value( string s_key, void *lpvalue )
{
    this->sess_values[s_key] = lpvalue;
}


void*
sess::get_elem( string s_key )
{
    return sess_values[s_key];
}

string
sess::dump()
{
    string s_ret = string(SESSDMP) + get_id();
    map<string, void*>::const_iterator iter;

    for (iter = sess_values.begin(); iter != sess_values.end(); iter++)
        s_ret = s_ret + "\nkey: " + iter->first;

    return s_ret;
}

#endif
