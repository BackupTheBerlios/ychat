SRCS=yc_away.cpp yc_col.cpp yc_getrusage.cpp yc_m.cpp yc_me.cpp yc_msg.cpp yc_q.cpp yc_s.cpp yc_template.cpp yc_time.cpp yc_topic.cpp yc_uptime.cpp yc_version.cpp
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
