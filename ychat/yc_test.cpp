#include <iostream>

extern "C" {
export int test(char *t);

int test(char *t){
std::cout << "Hallo" << t << std::endl;
}
}
