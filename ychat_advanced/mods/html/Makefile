SRCS= yc_colors.cpp yc_help.cpp yc_options.cpp yc_register.cpp
MODS=$(SRCS:.cpp=.so)
CC=g++
all: mods
$(MODS): $(SRCS)
	$(CC)  -shared -o $* `echo $* | sed s/.so//`.cpp
mods:	$(MODS)
	ls *.so
	ls *.so | wc -l
clean:
	rm *.so 
