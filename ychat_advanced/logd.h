#include "incl.h"

#ifndef LOGD_H
#define LOGD_H

#include <queue>
#include <time.h>

class logd
{

private:
    string s_logfile;
    queue<string> s_queue;
    int i_lines;
public:
    logd( string filename );
    ~logd();

    void flush();
    void log( map_string request );
};
#endif
