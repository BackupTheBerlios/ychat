#ifndef WRAP_H
#define WRAP_H

#include "chat.h"
#include "conf.h"
#include "data.h"
#include "gcol.h"
#include "html.h"
#include "lang.h"
#include "logd.h"
#include "mman.h"
#include "modl.h"
#include "mutx.h"
#include "ncur.h"
#include "sman.h"
#include "sock.h"
#include "timr.h"

using namespace std;

class dynamic_wrap 
{
 public: 
    chat* CHAT;
    conf* CONF;
    data* DATA;
    gcol* GCOL;
    html* HTML;
    lang* LANG;
    logd* LOGD;
    mman* MMAN;
    modl* MODL;
    mutx* MUTX;
    ncur* NCUR;
    sman* SMAN;
    sock* SOCK;
    timr* TIMR;
};

class wrap
{
public:
    static void system_message( char* c_message )
    { 
     wrap::system_message( string(c_message) );
    }
    static void system_message( string* p_message )
    { 
     wrap::system_message( *p_message );
    }
    static void system_message( string s_message );

    static chat* CHAT;
    static conf* CONF;
    static data* DATA;
    static gcol* GCOL;
    static html* HTML;
    static lang* LANG;
    static logd* LOGD;
    static mman* MMAN;
    static modl* MODL;
    static mutx* MUTX;
    static ncur* NCUR;
    static sman* SMAN;
    static sock* SOCK;
    static timr* TIMR;
    static dynamic_wrap* WRAP; 
};


#endif
