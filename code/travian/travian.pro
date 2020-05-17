TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    includes/gumbo/Document.h \
    includes/gumbo/Node.h \
    includes/gumbo/Object.h \
    includes/gumbo/Parser.h \
    includes/gumbo/QueryUtil.h \
    includes/gumbo/Selection.h \
    includes/gumbo/Selector.h \
    includes/app.h \
    includes/cmd_line.h \
    includes/connection.h \
    includes/defs.game.h \
    includes/defs.h \
    includes/defs.structs.h \
    includes/event.h \
    includes/player.h \
    includes/server.h \
    includes/village.h \


SOURCES += \
    main.cpp \
    src/gumbo/Document.cpp \
    src/gumbo/Node.cpp \
    src/gumbo/Object.cpp \
    src/gumbo/Parser.cpp \
    src/gumbo/QueryUtil.cpp \
    src/gumbo/Selection.cpp \
    src/gumbo/Selector.cpp \
    src/player.cpp \
    src/connection.cpp \
    src/cmd_line.cpp \
    src/app.cpp \
    src/defs.game.cpp \
    src/village.cpp \
    src/server.cpp


INCLUDEPATH += ./includes ./includes/gumbo
DEPENDPATH += ./includes ./includes/gumbo

USER_CXXFLAGS = -std=c++11 -Wall

LIBS += -L/usr/local/lib -lcurl -lgumbo
