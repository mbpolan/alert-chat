######################################################################
# Automatically generated by qmake (2.01a) wt. listopada 23 15:54:34 2010
######################################################################

QT += network
TEMPLATE = app
TARGET = 
DEPENDPATH += . src ui
INCLUDEPATH += . src
UI_DIR = ui
MOC_DIR = moc

# Input
HEADERS += src/logindialog.h src/mainwindow.h src/networkmanager.h src/packet.h src/protspec.h \
    src/preferencesdialog.h \
    src/configloader.h \
    src/registerdialog.h
FORMS += ui/logindialog.ui ui/mainwindow.ui \
    ui/preferencesdialog.ui \
    ui/registerdialog.ui
SOURCES += src/client.cpp \
           src/logindialog.cpp \
           src/mainwindow.cpp \
           src/networkmanager.cpp \
	   src/packet.cpp \
    src/preferencesdialog.cpp \
    src/configloader.cpp \
    src/registerdialog.cpp

RESOURCES += \
    resources.qrc
