all:    base modules stats
	@echo "Done! Now edit the conf.txt and run ./bin/ychat"
modules:
	gmake -C ./src/mods 
clean_modules:
	gmake -C ./src/mods clean
base:	
	perl ./scripts/buildnr.pl || ./scripts/noperl.sh
	gmake -C ./src
clean_base:
	gmake -C ./src clean
stats:
	perl scripts/stats.pl || ./scripts/noperl.sh
start:	base modules 
	./bin/ychat
gpl:
	more COPYING
cvsupdate:
	@echo "Updating to newest CVS version of ychat_advanced"
	@echo "Please just hit enter if you get asked about a password :)"
	touch ~/.cvspass 
	cvs -d:pserver:anonymous@cvs.ychat.berlios.de:/cvsroot/ychat login &&
	@echo "Backing up old configuration files" &&
	cp conf.txt conf.txt.old &&
	cp lang/en lang/en.old &&
	cp lang/de lang/de.old &&
	@echo "Running cvs update" &&
	cvs update &&
	cvs -d:pserver:anonymous@cvs.ychat.berlios.de:/cvsroot/ychat logout &&
	@echo "Done updating CVS sources!" &&
	perl ./scripts/mergeconf.pl || ./scripts/noperl.sh &&
	rm conf.txt.old lang/*.old
	@which cvs >/dev/null || ./scripts/nocvs.sh
clean:	clean_base clean_modules
help:
	@echo "You may run gmake with the following parameters:"
	@grep "^ gmake " README 
	@echo "For more questions read the README file or contact mail@ychat.org!"
