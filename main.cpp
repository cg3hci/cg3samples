/**
    @author Stefano Nuvoli
*/


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


#include <iostream>

int main()
{

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

    return 0;
}
