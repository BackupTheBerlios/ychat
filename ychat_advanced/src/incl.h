// Contains header files which are included by all classes.

// Include some std headers.
#include <iostream>

// Since thread synchronization is a big issue this header needs
// to be included  by every other file too.
#include <pthread.h>

#include <ncurses.h>

// std::string.
#include <string>

// Include all the custom global variables.
#include "glob.h"

// Include all the custom messages.
#include "msgs.h"

// Include wrapper
#include "wrap.h"
