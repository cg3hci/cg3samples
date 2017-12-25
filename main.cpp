/**
    @author Stefano Nuvoli
*/

///Test cg3 core module
#ifdef CG3_CORE_DEFINED
#include <cg3/data_structures/arrays/arrays.h>
#include <cg3/geometry/bounding_box.h>
#include <cg3/utilities/comparators.h>
#include <cg3/utilities/timer.h>
#endif

#include <cg3/cg3lib.h>

#ifdef BSTTEST
#include "tests/bsttest.h"
#endif

#ifdef RTTEST
#include "tests/rttest.h"
#endif

#ifdef AABBTEST
#include "tests/aabbtest.h"
#endif

#ifdef CHTEST
#include "tests/chtest.h"
#endif

#ifdef GRAPHTEST
#include "tests/graphtest.h"
#endif

#ifdef BSTSAMPLE
#include "samples/bstsample.h"
#endif

#ifdef RTSAMPLE
#include "samples/rtsample.h"
#endif

#ifdef AABBSAMPLE
#include "samples/aabbsample.h"
#endif

#ifdef CHSAMPLE
#include "samples/chsample.h"
#endif

#ifdef GRAPHSAMPLE
#include "samples/graphsample.h"
#endif

#ifdef BIPARTITEGRAPHSAMPLE
#include "samples/bipartitegraphsample.h"
#endif

#ifdef CH3DSAMPLE
#include "samples/ch3dsample.h"
#endif

#ifdef VIEWERSAMPLE
#include "samples/viewersample.h"
#endif


#include <iostream>

int main(int argc, char *argv[]) {
    CG3_SUPPRESS_WARNING(argc);
    CG3_SUPPRESS_WARNING(argv);

#ifdef BSTTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " BST test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    BSTTests::testCorrectness();
    BSTTests::testRandom();
    BSTTests::testMixed();
    BSTTests::testProgressive();

    std::cout << std::endl << std::endl;
#endif

#ifdef RTTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " RT test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    RTTests::testCorrectness();
    RTTests::testRandom();
    RTTests::testMixed();
    RTTests::testProgressive();

    std::cout << std::endl << std::endl;
#endif


#ifdef AABBTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " AABBTree test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    AABBTest::testCorrectness();
    AABBTest::testRandom();
    AABBTest::testMixed();
    AABBTest::testProgressive();

    std::cout << std::endl << std::endl;
#endif

#ifdef CHTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " Convex hull test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    CHTests::testCorrectness();
    CHTests::testRandom();
    CHTests::testMixed();
    CHTests::testProgressive();

    std::cout << std::endl << std::endl;
#endif

#ifdef GRAPHTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " Graph test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    GraphTests::testCorrectness();
    GraphTests::testDijkstra();

    std::cout << std::endl << std::endl;
#endif

#ifdef BSTSAMPLE
    std::cout << "------ Binary search tree sample ------" << std::endl << std::endl;
    BSTSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef RTSAMPLE
    std::cout << "------ Range tree sample ------" << std::endl << std::endl;
    RTSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef AABBSAMPLE
    std::cout << "------ AABBTree sample ------" << std::endl << std::endl;
    AABBSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef CHSAMPLE
    std::cout << "------ Convex hull sample ------" << std::endl << std::endl;
    CHSamples::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef GRAPHSAMPLE
    std::cout << "------ Graph sample ------" << std::endl << std::endl;
    GraphSamples::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef BIPARTITEGRAPHSAMPLE
    std::cout << "------ Bipartite Graph sample ------" << std::endl << std::endl;
    BipartiteGraphSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef CH3DSAMPLE
    std::cout << "------ Convex Hull 3D sample ------" << std::endl << std::endl;
    CH3DSamples::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef VIEWERSAMPLE
    return ViewerSample::viewer(argc, argv);
#else
    return 0;
#endif
}
