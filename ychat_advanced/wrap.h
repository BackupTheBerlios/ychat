#ifndef WRAP_H
#define WRAP_H

#include "chat.h"
#include "conf.h"
#include "html.h"
#include "lang.h"
#include "mman.h"
#include "modl.h"
#include "mutx.h"
#include "ncur.h"
#include "sman.h"
#include "sock.h"
#include "timr.h"

using namespace std;

class wrap
{
public:
    static chat* CHAT;
    static conf* CONF;
    static html* HTML;
    static lang* LANG;
    static mman* MMAN;
    static modl* MODL;
    static mutx* MUTX;
    static ncur* NCUR;
    static sman* SMAN;
    static sock* SOCK;
    static timr* TIMR;
};


#endif
