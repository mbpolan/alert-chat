bin_PROGRAMS = acserver

acserver_SOURCES = database.cpp database.h \
		   dbsqlite3.cpp dbsqlite3.h \
		   definitions.h \
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
AM_LDFLAGS = -lpthread -lsqlite3

