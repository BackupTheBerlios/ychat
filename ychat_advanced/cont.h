// class cont declaration. defines a simple data container class.


#include "incl.h"

#ifndef CONT_H
#define CONT_H

#include "hmap.h"

using namespace std;

class cont
{
protected:
    map_string map_vals;

public:
    cont::~cont();

    // small inline methods:
    void clear_vals()
    {
     // removes all values.
      map_vals.clear();
    } 

    // public methods:
    virtual string get_val( string s_key ); // get a specific map_vals value.
};

#endif
