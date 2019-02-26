TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/player.cpp \
    main.cpp \
    src/connection.cpp \
    src/cmd_line.cpp \
    src/app.cpp \
    src/defs.game.cpp \
    src/html.cpp

HEADERS += \
    includes/player.h \
    includes/connection.h \
    includes/event.h \
    includes/cmd_line.h \
    includes/defs.h \
    includes/defs.structs.h \
    includes/app.h \
    includes/defs.game.h \
    includes/rapidxml/rapidxml.hpp \
    includes/rapidxml/rapidxml_iterators.hpp \
    includes/rapidxml/rapidxml_print.hpp \
    includes/rapidxml/rapidxml_utils.hpp \
    includes/html.h

INCLUDEPATH += ./includes /usr/local/include/boost
DEPENDPATH += ./includes /usr/local/include/boost


USER_CXXFLAGS = -std=c++11 -Wall

LIBS += -L/usr/local/lib -lcurl -lboost_regex


