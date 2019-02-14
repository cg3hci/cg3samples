/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "chtest.h"

#include <iostream>
#include <random>
#include <assert.h>
#include <iomanip>

#include <vector>

#include "cg3/geometry/2d/point2d.h"
#include "cg3/geometry/segment.h"

#include "cg3/algorithms/2d/convexhull2d.h"
#include "cg3/algorithms/2d/convexhull2d_incremental.h"

#ifdef CG3_CGAL_DEFINED
#include "cg3/cgal/2d/convexhull2d.h"
#endif

#include <cg3/cg3lib.h>
#include <cg3/utilities/timer.h>

#define ITERATION 3
#define INDENTSPACE 12

#define INPUTSIZE 1000000
#define RANDOM_MAX (INPUTSIZE*100)



namespace CHTests {


/* ----- TYPEDEFS ----- */

typedef cg3::Point2Dd Point2D;



/* ----- FUNCTION DECLARATION ----- */


void printHeader();


void doTestsOnInput(std::vector<int>& testNumbers);


void testGrahamScan(std::vector<Point2D>& testPoints);
void testIncremental(std::vector<Point2D>& testPoints);

#ifdef CG3_CGAL_DEFINED
void testCGAL(std::vector<Point2D>& testPoints);
#endif

/* ----- IMPLEMENTATION ----- */

void testCorrectness() {
    std::vector<Point2D> points;
    std::list<Point2D> convexHull;

    cg3::IncrementalConvexHull<double> ch;
    std::list<Point2D> convexHullIncremental;

    //No data
    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 0);

    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 0);


    //1 point
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(0,0));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 1);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 1);


    //2 equal points
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(0,0));
    points.push_back(Point2D(0,0));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 1);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 1);


    //3 equal points
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(3,5));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 1);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 1);



    //5 equal points
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(3,5));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 1);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 1);


    //2 equal points and one different
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(0,5));
    points.push_back(Point2D(3,5));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 2);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 2);


    //3 collinear points
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(0,1));
    points.push_back(Point2D(0,5));
    points.push_back(Point2D(0,10));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 2);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 2);


    //Three not collinear points
    points.clear();
    convexHull.clear();
    points.push_back(Point2D(0,1));
    points.push_back(Point2D(0,5));
    points.push_back(Point2D(1,1));

    cg3::convexHull2D(points, convexHull);
    assert(convexHull.size() == 3);

    convexHullIncremental.clear();
    ch = cg3::IncrementalConvexHull<double>(points);
    ch.convexHull(std::back_inserter(convexHullIncremental));
    assert(convexHullIncremental.size() == 3);
}

void testRandom() {

    std::cout << " ------ RANDOM ------ " << std::endl << std::endl;

    printHeader();

    for (int t = 0; t < ITERATION; t++) {

        //Setup random generator
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type>
                distIn(0,RANDOM_MAX*2);

        std::vector<int> testNumbers;

        //Random test number generation
        for (int i = 0; i < INPUTSIZE*2; i++) {
            int randomValue = distIn(rng)-RANDOM_MAX;
            testNumbers.push_back(randomValue);
        }

        doTestsOnInput(testNumbers);
    }
}


void testMixed() {    
    std::cout << std::endl << " ------ MIXED VALUES VECTOR ------ " << std::endl << std::endl;

    printHeader();

    for (int t = 0; t < ITERATION; t++) {
        //Setup random generator
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type>
                distIn(0,RANDOM_MAX*2);

        std::vector<int> testNumbers;

        //Progressive number generation
        for (int i = 0; i < INPUTSIZE/2; i++) {
            testNumbers.push_back(i);
        }
        //Random test number generation
        for (int i = 0; i < INPUTSIZE/2; i++) {
            int randomValue = distIn(rng)-RANDOM_MAX;
            testNumbers.push_back(randomValue);
        }
        //Progressive reversed number generation
        for (int i = 0; i < INPUTSIZE/2; i++) {
            testNumbers.push_back(INPUTSIZE*2-i);
        }
        //Random test number generation
        for (int i = 0; i < INPUTSIZE/2; i++) {
            int randomValue = distIn(rng)-RANDOM_MAX;
            testNumbers.push_back(randomValue);
        }

        doTestsOnInput(testNumbers);
    }
}


void testProgressive() {
    std::cout << std::endl << " ------ REVERSE SORTED VECTOR ------ " << std::endl << std::endl;

    printHeader();

    for (int t = 0; t < ITERATION; t++) {
        std::vector<int> testNumbers;

        //Progressive reversed number generation
        for (int i = 0; i < INPUTSIZE*2; i++) {
            testNumbers.push_back(INPUTSIZE*2-i);
        }

        doTestsOnInput(testNumbers);
    }
}


/* ----- FUNCTION DECLARATION ----- */

void printHeader() {
    std::cout <<
         std::setw(INDENTSPACE) << std::left << "METHOD" <<
         std::setw(INDENTSPACE) << std::left << "INPUTSIZE" <<
         std::setw(INDENTSPACE) << std::left << "ALGORITHM" <<
         std::setw(INDENTSPACE) << std::left << "CHSIZE" <<
         std::setw(INDENTSPACE) << std::left << "TOTAL" <<
         std::endl << std::endl;
}


void doTestsOnInput(std::vector<int> &testNumbers)
{    
    std::vector<Point2D> testPoints;

    for (size_t i = 0; i < testNumbers.size()-1; i+=2) {
        Point2D p(testNumbers.at(i), testNumbers.at(i+1));
        testPoints.push_back(p);
    }

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "GRAHAM";
    testGrahamScan(testPoints);

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "INCREMENTAL";
    testIncremental(testPoints);

#ifdef CG3_CGAL_DEFINED
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "CGAL";
    testCGAL(testPoints);
#endif

    std::cout << std::endl;

    testPoints.clear();
}






void testGrahamScan(std::vector<Point2D>& testPoints)
{
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << testPoints.size();

    cg3::Timer totalTimer("Total");
    cg3::Timer timer("Step");

    totalTimer.start();


    timer.start();

    std::list<Point2D> convexHull;
    cg3::convexHull2D(testPoints, convexHull);

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    /* CH size */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << convexHull.size();



    /* Total */

    totalTimer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << totalTimer.delay();


    //Check if it is a convex hull
    bool error = false;

    typename std::list<Point2D>::iterator it2 = convexHull.end();
    typename std::list<Point2D>::iterator it3 = convexHull.end();
    for (typename std::list<Point2D>::iterator it1 = convexHull.begin(); it1 != convexHull.end(); it1++) {
        if (it2 != convexHull.end() && it3 != convexHull.end()) {
            const cg3::Segment<Point2D> seg(*it1, *it2);
            const Point2D& point = *it3;

            if (cg3::isPointAtLeft(seg, point)) {
                error = true;
            }
        }
        it3 = it2;
        it2 = it1;
    }
    if (error) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "ERROR!";
    }
//    assert(!error);


    std::cout << std::endl;
}



void testIncremental(std::vector<Point2D>& testPoints)
{
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << testPoints.size();


    cg3::Timer totalTimer("Total");
    cg3::Timer timer("Step");

    totalTimer.start();


    timer.start();

    cg3::IncrementalConvexHull<double> incrementalConvexHull;
    for (Point2D p : testPoints)
        incrementalConvexHull.addPoint(p);

    std::list<Point2D> convexHull;
    incrementalConvexHull.convexHull(std::back_inserter(convexHull));

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();

    /* CH size */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << convexHull.size();


    /* Total */

    totalTimer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << totalTimer.delay();


    bool error = false;

    typename std::list<Point2D>::iterator it2 = convexHull.end();
    typename std::list<Point2D>::iterator it3 = convexHull.end();
    for (typename std::list<Point2D>::iterator it1 = convexHull.begin(); it1 != convexHull.end(); it1++) {
        if (it2 != convexHull.end() && it3 != convexHull.end()) {
            const cg3::Segment<Point2D> seg(*it1, *it2);
            const Point2D& point = *it3;

            if (cg3::isPointAtLeft(seg, point)) {
                error = true;
            }
        }
        it3 = it2;
        it2 = it1;
    }
    if (error) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "ERROR!";
    }
//    assert(!error);


    std::cout << std::endl;
}

#ifdef CG3_CGAL_DEFINED
void testCGAL(std::vector<Point2D>& testPoints)
{
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << testPoints.size();

    cg3::Timer totalTimer("Total");
    cg3::Timer timer("Step");

    totalTimer.start();


    timer.start();

    std::list<Point2D> convexHull;
    cg3::cgal::convexHull2D(testPoints, convexHull);

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    /* CH size */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << convexHull.size();


    /* Total */

    totalTimer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << totalTimer.delay();


    bool error = false;

    typename std::list<Point2D>::iterator it2 = convexHull.end();
    typename std::list<Point2D>::iterator it3 = convexHull.end();
    for (typename std::list<Point2D>::iterator it1 = convexHull.begin(); it1 != convexHull.end(); it1++) {
        if (it2 != convexHull.end() && it3 != convexHull.end()) {
            const cg3::Segment<Point2D> seg(*it1, *it2);
            const Point2D& point = *it3;

            if (cg3::isPointAtLeft(seg, point)) {
                error = true;
            }
        }
        it3 = it2;
        it2 = it1;
    }
    if (error) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "ERROR!";
    }
    assert(!error);



    std::cout << std::endl;

}
#endif




}
