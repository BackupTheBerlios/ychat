#include "incl.h"

#ifndef SESS_H
#define SESS_H

#include <map>
#include <string>

class cont;

using namespace std;

typedef map<string, void*> sess_map;

class sess : public name
{

private:
    sess_map sess_values;
    string sess_id;

public:
    sess(string s_id);
    ~sess();

    string get_id();
    void set_value(string s_key, void *lpvalue);
    void *get_elem( string s_key );
    void invalidate();
    string dump();
};
#endif
