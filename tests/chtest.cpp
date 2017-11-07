/**
    @author Stefano Nuvoli
*/
#include "bsttest.h"

#include <iostream>
#include <random>
#include <chrono>
#include <assert.h>
#include <iomanip>

#include <set>
#include <vector>

#include "cg3/geometry/2d/point2d.h"
#include "cg3/geometry/segment.h"

#include "cg3/algorithms/convexhull2d.h"

#include "cg3/cgal/cgal_convexhull.h"

#define ITERATION 5
#define INDENTSPACE 12

#define INPUTSIZE 1000000
#define RANDOM_MAX (INPUTSIZE*100)



namespace ConvexHullTests {


/* ----- TYPEDEFS ----- */

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef high_resolution_clock::time_point time_point;

typedef cg3::Point2Dd Point2D;



/* ----- FUNCTION DECLARATION ----- */


void printHeader();


void test(std::vector<int>& testNumbers);


void testGrahamScan(std::vector<Point2D>& testPoints);
void testCGAL(std::vector<Point2D>& testPoints);


/* ----- FUNCTION IMPLEMENTATION ----- */

void printHeader() {
    std::cout <<
         std::setw(INDENTSPACE) << std::left << "METHOD" <<
         std::setw(INDENTSPACE) << std::left << "INPUTSIZE" <<
         std::setw(INDENTSPACE) << std::left << "ALGORITHM" <<
         std::setw(INDENTSPACE) << std::left << "CHSIZE" <<
         std::setw(INDENTSPACE) << std::left << "TOTAL" <<
         std::endl << std::endl;
}



void testRandom() {

    std::cout << " ------ RANDOM ------ " << std::endl << std::endl;

    printHeader();

    for (int t = 0; t < ITERATION; t++) {

        //Setup random generator
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type>
                distIn(-RANDOM_MAX,RANDOM_MAX);

        std::vector<int> testNumbers;

        //Random test number generation
        for (int i = 0; i < INPUTSIZE; i++) {
            int randomValue = distIn(rng);
            testNumbers.push_back(randomValue);
        }

        test(testNumbers);
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
                distIn(-RANDOM_MAX,RANDOM_MAX);

        std::vector<int> testNumbers;

        //Progressive number generation
        for (int i = 0; i < INPUTSIZE/4; i++) {
            testNumbers.push_back(i);
        }
        //Random test number generation
        for (int i = 0; i < INPUTSIZE/4; i++) {
            int randomValue = distIn(rng);
            testNumbers.push_back(randomValue);
        }
        //Progressive reversed number generation
        for (int i = 0; i < INPUTSIZE/4; i++) {
            testNumbers.push_back(INPUTSIZE-i);
        }
        //Random test number generation
        for (int i = 0; i < INPUTSIZE/4; i++) {
            int randomValue = distIn(rng);
            testNumbers.push_back(randomValue);
        }

        test(testNumbers);
    }
}


void testProgressive() {
    std::cout << std::endl << " ------ REVERSE SORTED VECTOR ------ " << std::endl << std::endl;

    printHeader();

    for (int t = 0; t < ITERATION; t++) {
        std::vector<int> testNumbers;

        //Progressive reversed number generation
        for (int i = 0; i < INPUTSIZE; i++) {
            testNumbers.push_back(INPUTSIZE-i);
        }

        test(testNumbers);
    }
}



void test(std::vector<int> &testNumbers)
{    
    std::vector<Point2D> testPoints;

    for (size_t i = 0; i < testNumbers.size(); i++) {
        Point2D p(testNumbers.at(i), testNumbers.at(testNumbers.size()-1-i));
        testPoints.push_back(p);
    }

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "GRAHAM";
    testGrahamScan(testPoints);

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "CGAL";
    testGrahamScan(testPoints);


    std::cout << std::endl;

    testPoints.clear();
}






void testGrahamScan(std::vector<Point2D>& testPoints)
{
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << testPoints.size();

    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    t1 = high_resolution_clock::now();

    std::list<Point2D> convexHull;
    cg3::getConvexHull2D(testPoints, convexHull);

    t2 = high_resolution_clock::now();


    auto algorithmTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) algorithmTime/1000;


    /* CH size */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << convexHull.size();


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    std::cout << std::endl;



    bool error = false;

    typename std::list<Point2D>::iterator it2 = convexHull.end();
    typename std::list<Point2D>::iterator it3 = convexHull.end();
    for (typename std::list<Point2D>::iterator it1 = convexHull.begin(); it1 != convexHull.end(); it1++) {
        if (it2 != convexHull.end() && it3 != convexHull.end()) {
            const cg3::Segment<Point2D> seg(*it1, *it2);
            const Point2D& point = *it3;

            if (cg3::isPointAtRight(seg, point)) {
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

}


void testCGAL(std::vector<Point2D>& testPoints)
{
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << testPoints.size();

    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    t1 = high_resolution_clock::now();

    std::list<Point2D> convexHull;
    cg3::cgal::getConvexHull2D(testPoints, convexHull);

    t2 = high_resolution_clock::now();


    auto algorithmTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) algorithmTime/1000;


    /* CH size */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << convexHull.size();


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    std::cout << std::endl;



    bool error = false;

    typename std::list<Point2D>::iterator it2 = convexHull.end();
    typename std::list<Point2D>::iterator it3 = convexHull.end();
    for (typename std::list<Point2D>::iterator it1 = convexHull.begin(); it1 != convexHull.end(); it1++) {
        if (it2 != convexHull.end() && it3 != convexHull.end()) {
            const cg3::Segment<Point2D> seg(*it1, *it2);
            const Point2D& point = *it3;

            if (cg3::isPointAtRight(seg, point)) {
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

}





}
