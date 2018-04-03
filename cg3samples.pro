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
CONFIG += CG3_CORE CG3_DATA_STRUCTURES CG3_ALGORITHMS CG3_MESHES CG3_VIEWER CG3_CGAL
CG3_VIEWER {
    CONFIG += qt
}
include (cg3lib/cg3.pri)
message($$MODULES)

#uncomment if you want to include tests
CONFIG += TESTS
#uncomment if you want to include samples
CONFIG += SAMPLES

DEFINES += "CG3_SAMPLES_HOME='$$_PRO_FILE_PWD_'"

TESTS {
    DEFINES += TESTS

    #uncomment if you want to run BST tests
    DEFINES += BSTTEST
    #uncomment if you want to run Range tree tests
    DEFINES += RTTEST
    #uncomment if you want to run AABBTree tests
    DEFINES += AABBTEST
    #uncomment if you want to run convex hull tests
    DEFINES += CHTEST
    #uncomment if you want to run convex hull tests
    DEFINES += GRAPHTEST



    SOURCES += \
        tests/bsttest.cpp \
        tests/rttest.cpp \
        tests/aabbtest.cpp \
        tests/chtest.cpp \
        tests/graphtest.cpp


    HEADERS += \
        tests/bsttest.h \
        tests/rttest.h \
        tests/aabbtest.h \
        tests/chtest.h \
        tests/graphtest.h
}
SAMPLES {
    DEFINES += SAMPLES

    #uncomment if you want to run BST tests
    DEFINES += BSTSAMPLE
    #uncomment if you want to run Range tree tests
    DEFINES += RTSAMPLE
    #uncomment if you want to run AABBTree tests
    DEFINES += AABBSAMPLE
    #uncomment if you want to run convex hull tests
    DEFINES += CHSAMPLE
    #uncomment if you want to run graph tests
    DEFINES += GRAPHSAMPLE
    #uncomment if you want to run bipartite graph tests
    DEFINES += BIPARTITEGRAPHSAMPLE
    #uncomment if you want to run convex hull 3d tests
    DEFINES += CH3DSAMPLE
    #uncomment if you want to run viewer tests
    DEFINES += VIEWERSAMPLE

    HEADERS += \
        samples/bstsample.h \
        samples/rtsample.h \
        samples/aabbsample.h \
        samples/chsample.h \
        samples/graphsample.h \
        samples/bipartitegraphsample.h \
        samples/ch3dsample.h \
        samples/viewersample.h

    SOURCES += \
        samples/bstsample.cpp \
        samples/rtsample.cpp \
        samples/aabbsample.cpp \
        samples/chsample.cpp \
        samples/graphsample.cpp \
        samples/bipartitegraphsample.cpp \
        samples/ch3dsample.cpp \
        samples/viewersample.cpp
}
