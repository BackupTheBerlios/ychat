#ifndef WRAP_CPP
#define WRAP_CPP

#include "wrap.h"

using namespace std;

chat* wrap::CHAT;
conf* wrap::CONF;
data* wrap::DATA;
gcol* wrap::GCOL;
html* wrap::HTML;
lang* wrap::LANG;
logd* wrap::LOGD;
mman* wrap::MMAN;
modl* wrap::MODL;
mutx* wrap::MUTX;
ncur* wrap::NCUR;
sman* wrap::SMAN;
sock* wrap::SOCK;
timr* wrap::TIMR;
dynamic_wrap* wrap::WRAP;

void
wrap::system_message( string s_message )
{
#ifdef NCURSES
   wrap::NCUR->print( s_message );
#endif
#ifdef SERVMSG
   pthread_mutex_lock  ( &wrap::MUTX->mut_stdout );
   cout << s_message << endl;
   pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
#endif
   wrap::LOGD->log_simple_line( s_message + "\n" );
}
#endif
