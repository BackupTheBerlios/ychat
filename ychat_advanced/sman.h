#ifndef SMAN_H
#define SMAN_H

#include "incl.h"
#include "hmap.h"
#include "sess.h"
#include "s_tool.h"
#include "s_conf.h"
#include <cstdlib>

using namespace std;

class sman{

	private:
		hmap<sess *, string> *sessions;
		string generate_id( int len );
		int session_count;	
	public:
		sman();
		~sman();
		sess *get_session( string s_id );
		int get_session_count( ) { return this->session_count; }
		sess *create_session( );
		void destroy_session( string s_id );


};


#endif

