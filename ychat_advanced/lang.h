#include "incl.h"

#ifndef LANG_H
#define LANG_H

#include "cont.h"
#include "name.h"

using namespace std;

class lang : public cont, name
{
private:

public:
    // public methods:
    lang ( string s_lang ); // standard constructor.
    ~lang(); 		 // standard destructor.

    virtual void parse( );    // parses the config file.
};

#endif
