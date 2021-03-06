// class conf declaration. this class parses the server config file.

#ifndef CONF_H
#define CONF_H

class conf;

#include "incl.h"
#include "smap.h"
#include "name.h"

using namespace std;

class conf : public nmap<string,string>, name
{
private:

public:
    // public methods:
    conf ( string s_conf ); // standard constructor.
    ~conf(); 		 // standard destructor.

    virtual void parse( );    // parses the config file.
};

#endif
