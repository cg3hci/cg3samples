CONFIG(debug, debug|release){
    DEFINES += DEBUG
}

CONFIG(release, debug|release){
    DEFINES -= DEBUG
    #just uncomment next lines if you want to ignore asserts and got a more optimized binary
    CONFIG += FINAL_RELEASE
}

FINAL_RELEASE {
    unix:!macx{
        QMAKE_CXXFLAGS_RELEASE -= -g -O2
        QMAKE_CXXFLAGS += -Os -DNDEBUG
    }
}

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    samples/aabbsample.cpp \
    tests/aabbtest.cpp

#cg3lib module
include (cg3lib/cg3.pri)

#uncomment if you want to include test programs
CONFIG += TESTS
#uncomment if you want to include sample programs
CONFIG += SAMPLES


TESTS {
    DEFINES += TESTS

    #uncomment if you want to run BST test programs
    DEFINES += BSTTEST
    #uncomment if you want to run Range tree test programs
    DEFINES += RTTEST
    #uncomment if you want to run AABBTree test programs
    DEFINES += AABBTEST
    #uncomment if you want to run convex hull test programs
    DEFINES += CHTEST



    SOURCES += \
        tests/bsttest.cpp \
        tests/rttest.cpp \
        tests/chtest.cpp


    HEADERS += \
        tests/bsttest.h \
        tests/rttest.h \
        tests/chtest.h
}
SAMPLES {
    DEFINES += SAMPLES

    #uncomment if you want to run BST test programs
    DEFINES += BSTSAMPLE
    #uncomment if you want to run Range tree test programs
    DEFINES += RTSAMPLE
    #uncomment if you want to run AABBTree test programs
    DEFINES += AABBSAMPLE
    #uncomment if you want to run convex hull test programs
    DEFINES += CHSAMPLE


    SOURCES += \
        samples/bstsample.cpp \
        samples/rtsample.cpp \
        samples/chsample.cpp


    HEADERS += \
        samples/bstsample.h \
        samples/rtsample.h \
        samples/chsample.h
}

HEADERS += \
    samples/aabbsample.h \
    tests/aabbtest.h


