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
start:	base modules 
	./bin/ychat
gpl:
	more COPYING
update:
	@echo "Updating to newest CVS version of ychat_advanced"
	@echo "Please just hit enter if you get asked about a password :)"
	touch ~/.cvspass
	cvs -d:pserver:anonymous@cvs.ychat.berlios.de:/cvsroot/ychat login
	cvs update
	cvs -d:pserver:anonymous@cvs.ychat.berlios.de:/cvsroot/ychat logout
	@echo "Done updating CVS sources!"
clean:	clean_base clean_modules

