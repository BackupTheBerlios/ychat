all:    base modules stats
modules:
	gmake -C ./src/mods 
clean_modules:
	gmake -C ./src/mods clean
base:	
	perl ./scripts/buildnr.pl || ./scripts/noperl.sh
	cd ..
	gmake -C ./src
clean_base:
	gmake -C ./src clean
stats:
	perl scripts/stats.pl > stats.txt || ./scripts/noperl.sh
	cat stats.txt 2>/dev/null
clean:	clean_base clean_modules

