TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += ../travian/includes
INCLUDEPATH += ../travian/includes/gumbo
INCLUDEPATH += googletest/include googletest

DEPENDPATH += ../travian/includes
DEPENDPATH += ../travian/includes/gumbo
DEPENDPATH += googletest/include googletest

SOURCES += \
    main.cpp \
    googletest/src/gtest.cc \
    googletest/src/gtest-all.cc \
    googletest/src/gtest-death-test.cc \
    googletest/src/gtest-filepath.cc \
    googletest/src/gtest-matchers.cc \
    googletest/src/gtest-port.cc \
    googletest/src/gtest-printers.cc \
    googletest/src/gtest-test-part.cc \
    googletest/src/gtest-typed-test.cc \
    ../travian/src/gumbo/Document.cpp \
    ../travian/src/gumbo/Node.cpp \
    ../travian/src/gumbo/Object.cpp \
    ../travian/src/gumbo/Parser.cpp \
    ../travian/src/gumbo/QueryUtil.cpp \
    ../travian/src/gumbo/Selection.cpp \
    ../travian/src/gumbo/Selector.cpp \
    ../travian/src/app.cpp \
    ../travian/src/cmd_line.cpp \
    ../travian/src/connection.cpp \
    ../travian/src/defs.game.cpp \
    ../travian/src/player.cpp \
    ../travian/src/server.cpp \
    ../travian/src/village.cpp \
    src/def.structs.cpp \
    src/def.cpp \
    src/test_server.cpp


LIBS += -L/usr/local/lib -lcurl -lgumbo

HEADERS += \
    ../travian/includes/app.h \
    ../travian/includes/cmd_line.h \
    ../travian/includes/connection.h \
    ../travian/includes/defs.game.h \
    ../travian/includes/defs.h \
    ../travian/includes/defs.structs.h \
    ../travian/includes/event.h \
    ../travian/includes/player.h \
    ../travian/includes/server.h \
    ../travian/includes/village.h \
    ../travian/includes/gumbo/Document.h \
    ../travian/includes/gumbo/Node.h \
    ../travian/includes/gumbo/Object.h \
    ../travian/includes/gumbo/Parser.h \
    ../travian/includes/gumbo/QueryUtil.h \
    ../travian/includes/gumbo/Selection.h \
    ../travian/includes/gumbo/Selector.h \
    ./includes/test_server.h
