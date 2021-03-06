#ifndef THRD_CPP
#define THRD_CPP

#include "thrd.h"

using namespace std;

thrd::thrd( int i_sock )
{
    this->i_sock = i_sock;
}

thrd::~thrd()
{
    shutdown ( get_sock() , 2 );
    close    ( get_sock()     );
}

void
thrd::run()
{
    wrap::SOCK->read_write( this, i_sock );
}

#endif
