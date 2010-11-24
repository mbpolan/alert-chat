bin_PROGRAMS = acserver

acserver_SOURCES = definitions.h \
		   packet.cpp packet.h \
		   protocol.cpp protocol.h \
		   protspec.h \
		   server.cpp \
		   serversocket.cpp serversocket.h \
		   threadpool.cpp threadpool.h \
		   thread.h \
		   user.cpp user.h \
		   usermanager.cpp usermanager.h

AM_CPPFLAGS = -D__LINUX__
AM_LDFLAGS = -lpthread
