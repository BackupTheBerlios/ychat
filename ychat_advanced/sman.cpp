#ifndef SMAN_CXX
#define SMAN_CXX

#include "sman.h"

sman::sman()
{
	this->sessions=new hmap<sess *, string>(80);
	this->session_count=0;
}
sman::~sman()
{
	delete this->sessions;
}
string sman::generate_id( int len )
{
	string valid_chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
	string s_ret="";
	srand(time(0)+160682);
	for(int i=0;i<len;i++)
	{
		int i_char=rand() % 64;
		s_ret+=valid_chars[i_char];
	}
	return s_ret;
}
sess *sman::create_session( )
{
	string new_id=this->generate_id(s_tool::string2int( s_conf::get().get_val( "SESSION_LENGTH" ) ) ); 

	sess *new_sess= new sess( new_id  );

	this->session_count++;
	this->sessions->add_elem( new_sess, new_id );
	
	return new_sess;
}

sess *sman::get_session( string s_id )
{
	return this->sessions->get_elem( s_id );
}
void sman::destroy_session( string s_id )
{
	this->session_count--;
	this->sessions->del_elem( s_id );
}
#endif
