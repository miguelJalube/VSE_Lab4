TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14

config_yta {
    INCLUDEPATH += libs/pistache/include
    LIBS += $$PWD/libs/pistache/build/src/libpistache.a
}
else {
    LIBS += /usr/local/lib/libpistache.a
}



LIBS += -lpthread

SOURCES += \
        computationnode.cpp \
        getnbcomputenode.cpp \
        main.cpp \
        resetnbcomputenode.cpp

HEADERS += \
    computationnode.h \
    getnbcomputenode.h \
    resetnbcomputenode.h

config_fpgasimulator {
    SOURCES += fpgaaccesssimulator.cpp
    HEADERS += fpgaaccesssimulator.h
}
else {
    config_fpgaremote {
        SOURCES += fpgaaccess.cpp
        HEADERS += fpgaaccess.h
    }
    else {
        SOURCES += fpgaaccessremote.cpp
        HEADERS += fpgaaccessremote.h
    }
}
