#include <limits.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
typedef int function (char *t);


int main()
{
	void *module=NULL;
	function *test=NULL;


	module=dlopen("/home/vrichter/ychat/yc_test.so", RTLD_NOW);
	printf("dlerror: %s\n", dlerror());

	test=(function*)dlsym(module, "test");
        printf("dlerror: %s\n", dlerror());	
	int result = (*test)(" Frosta ist für alle da");

	dlclose( module );
}
