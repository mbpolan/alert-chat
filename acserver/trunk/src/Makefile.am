bin_PROGRAMS = acserver

acserver_SOURCES = definitions.h \
		   server.cpp \
		   serversocket.cpp serversocket.h \
		   threadpool.cpp threadpool.h \
		   thread.h \
		   usermanager.cpp usermanager.h

AM_CPPFLAGS = -D__LINUX__
AM_LDFLAGS = -lpthread
