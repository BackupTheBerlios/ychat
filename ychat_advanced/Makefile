all:    base modules stats
	@echo "Done! Now edit the conf.txt and run ./bin/ychat"
modules:
	gmake -C ./src/mods 
clean_modules:
	gmake -C ./src/mods clean
base:	
	@echo backing up old binary
	if test -f bin/ychat; then if test -f bin/ychat.old; then rm -f bin/ychat.old; fi; mv bin/ychat bin/ychat.old; fi
	@perl ./scripts/buildnr.pl || ./scripts/noperl.sh
	@perl ./scripts/setglobvals.pl || ./scripts/noperl.sh
	gmake -C ./src 
clean_base:
	gmake -C ./src clean
stats:
	@perl scripts/stats.pl || ./scripts/noperl.sh
start:	base modules 
	./bin/ychat
gpl:
	more COPYING
cvsupdate:
	@echo "Updating to newest CVS version of ychat_advanced"
	export CVSROOT=`cat CVS/Root` 
	export CVSUSER=`echo ${CVSROOT} | cut -d":" -f3 | cut -d"@" -f1`
	if test ${CVSUSER} == "anonymous"; then echo "/1 :pserver:${CVSUSER}@cvs.ychat.berlios.de:2401/cvsroot/ychat A" >~/.cvspass; fi &&
	@echo "Backing up old configuration files" &&
	cp conf.txt conf.txt.old &&
	cp lang/en lang/en.old &&
	cp lang/de lang/de.old &&
	@echo "Running cvs update" &&
	if test -f src/msgs.h; then mv src/msgs.h src/msgs.h.old; fi &&
	cvs update &&
	cvs logout &&
	@echo "Done updating CVS sources!" &&
	@perl ./scripts/mergeconf.pl || ./scripts/noperl.sh &&
	rm conf.txt.old lang/*.old
	@which cvs >/dev/null || ./scripts/nocvs.sh
clean:	clean_base clean_modules
help:
	@echo "You may run gmake with the following parameters:"
	@grep "^ gmake " README 
	@echo "For more questions read the README file or contact mail@ychat.org!"
