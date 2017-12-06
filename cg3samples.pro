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
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp

#cg3lib module
CONFIG += CG3_CORE CG3_DATA_STRUCTURES CG3_DCEL CG3_ALGORITHMS CG3_CGAL CG3_VIEWER
include (cg3lib/cg3.pri)
message($$MODULES)

#uncomment if you want to include test programs
CONFIG += TESTS
#uncomment if you want to include sample programs
CONFIG += SAMPLES

DEFINES += "CG3_SAMPLES_HOME='$$_PRO_FILE_PWD_'"

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
    #uncomment if you want to run convex hull test programs
    DEFINES += VIEWERTEST



    SOURCES += \
        tests/bsttest.cpp \
        tests/rttest.cpp \
        tests/chtest.cpp \
        tests/aabbtest.cpp


    HEADERS += \
        tests/bsttest.h \
        tests/rttest.h \
        tests/chtest.h \
        tests/aabbtest.h
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
    #uncomment if you want to run bipartite graph test programs
    DEFINES += BIPARTITEGRAPHSAMPLE
    #uncomment if you want to run convex hull 3d test programs
    DEFINES += CH3DSAMPLE

    HEADERS += \
        samples/bstsample.h \
        samples/rtsample.h \
        samples/chsample.h \
        samples/aabbsample.h \
        samples/bipartitegraphsample.h \
        samples/ch3dsamples.h

    SOURCES += \
        samples/bstsample.cpp \
        samples/rtsample.cpp \
        samples/chsample.cpp \
        samples/aabbsample.cpp \
        samples/bipartitegraphsample.cpp \
        samples/ch3dsamples.cpp
}
