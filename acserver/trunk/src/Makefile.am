bin_PROGRAMS = acserver

pkgsysconfdir = $(sysconfdir)/acserver
pkgsysconf_DATA = data.db server.conf

acserver_SOURCES = configmanager.cpp configmanager.h \
		   database.cpp database.h \
		   dbsqlite3.cpp dbsqlite3.h \
		   definitions.h \
		   logwriter.cpp logwriter.h \
		   packet.cpp packet.h \
		   protocol.cpp protocol.h \
		   protspec.h \
		   server.cpp \
		   serversocket.cpp serversocket.h \
		   threadpool.cpp threadpool.h \
		   thread.h \
		   user.cpp user.h \
		   usermanager.cpp usermanager.h \
		   variable.cpp variable.h

AM_CPPFLAGS = -D__LINUX__
AM_LDFLAGS = -lpthread -lsqlite3

