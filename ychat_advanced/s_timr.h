#ifndef S_TIMR_H
#define S_TIMR_H

#include "timr.h"

using namespace std;

class s_timr
{
private:
    static timr* obj;

public:
    static void init()
    {
        obj = new timr();
    }

    static timr& get
        ()
    {
        return *obj;
    }
};

#endif
