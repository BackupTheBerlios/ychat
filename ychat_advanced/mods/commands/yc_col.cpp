#include <iostream>
#include "../../user.h"
#include "../../s_tool.cpp"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);
 int valid_color( string );
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	string *sCommandLine=(string *)c->elem[0]; 	// contains the whole line
	user *p_user = (user*)c->elem[1];		// the corresponding user
	str_vector *params=(str_vector*)c->elem[2];	// param array

	
	string sColor=(string)params->front();
	sColor=s_tool::to_lower(sColor);
	if(valid_color(sColor)==1)	
	{  	

		string *answerstring=new string(p_user->get_name()+ " changes color to " + sColor + "<br>");
		p_user->msg_post( answerstring );
		p_user->set_col1(sColor);
	}else
	{
		string *answerstring=new string(sColor + " is not a valid color.<br>");
		p_user->msg_post( answerstring );
	}
 }
 int valid_color( string sCol ){
	
	if(sCol.size()!=6) return 0;
	string valid="abcdef0123456789";
	for(int i=0;i<sCol.size();i++)
	{
		string sChar=sCol.substr(i,1);
		if(valid.find(sChar)==string::npos)
		return 0;
	}	
	return 1;
 }

}

