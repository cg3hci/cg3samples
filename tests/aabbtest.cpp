/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "aabbtest.h"

#include <iostream>
#include <random>
#include <chrono>
#include <assert.h>
#include <iomanip>

#include <set>
#include <vector>

#include "cg3/geometry/2d/point2d.h"
#include "cg3/geometry/segment.h"

#include "cg3/data_structures/trees/aabbtree.h"

#define ITERATION 1
#define INDENTSPACE 12

#define INPUTSIZE 5000
#define RANDOM_MAX (INPUTSIZE*100)
#define MAXLENGTH RANDOM_MAX/100
#define QUERY_RANDOM_DIV 10
#define ONLYEFFICIENT (INPUTSIZE > 10000)

namespace AABBTest {

/* ----- TYPEDEFS ----- */

typedef cg3::Segment<int> Segment1D;
typedef cg3::Point2D<int> Point2D;
typedef cg3::Segment<Point2D> Segment2D;

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef high_resolution_clock::time_point time_point;


template <int D, class T> using AABBTree = typename cg3::AABBTree<D,T>;

typedef cg3::AABBValueType AABBValueType;


/* ----- FUNCTION DECLARATION ----- */

void printHeader();


bool aabbOverlap(const Segment1D& segment1, const Segment1D& segment2);
bool aabbOverlap(const Segment2D& segment1, const Segment2D& segment2);

double aabbValueExtractor(const Segment1D& segment, const AABBValueType& valueType, const int& dim);
double aabbValueExtractor(const Segment2D& segment, const AABBValueType& valueType, const int& dim);

bool segmentIntersection(const Segment1D& segment1, const Segment1D& segment2);
bool segmentIntersection(const Segment2D& segment1, const Segment2D& segment2);


void doTestsOnInput(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);

void testBrute1D(std::vector<Segment1D>& testSegments, std::vector<Segment1D>& randomSegments);
void testAABBTree1D(std::vector<Segment1D>& testSegments, std::vector<Segment1D>& randomSegments);

void testBrute2D(std::vector<Segment2D>& testSegments, std::vector<Segment2D>& randomSegments);
void testAABBTree2D(std::vector<Segment2D>& testSegments, std::vector<Segment2D>& randomSegment);


/* ----- IMPLEMENTATION ----- */

void testCorrectness() {
    AABBTree<2, Segment2D> tree1(&aabbValueExtractor);
    AABBTree<2, Segment2D> tree2(&aabbValueExtractor);

    tree1.insert(Segment2D(Point2D(1,2), Point2D(2,4)));
    tree1.insert(Segment2D(Point2D(1,22), Point2D(3,10)));
    tree1.insert(Segment2D(Point2D(10,3), Point2D(4,10)));
    tree1.insert(Segment2D(Point2D(1,6), Point2D(10,5)));

    //Check copy constructor and move constructor
    tree2 = tree1;
    tree2.clear();

    std::vector<Segment2D> vec;
    for (int i = 0; i < 2000; i+=4)
        vec.push_back(Segment2D(Point2D(i,i+1), Point2D(i+2,i+3)));

    tree2 = AABBTree<2, Segment2D>(vec, &aabbValueExtractor);
    tree2.clear();

    AABBTree<2, Segment2D> tree3(
                std::move(
                    AABBTree<2, Segment2D>(vec, &aabbValueExtractor)));
    tree3.clear();
}

void testRandom() {

    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(-RANDOM_MAX,RANDOM_MAX);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Random test number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        testNumbers.push_back(randomValue);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        randomNumbers.push_back(randomValue/QUERY_RANDOM_DIV);
    }


    std::cout << " ------ RANDOM ------ " << std::endl << std::endl;

    doTestsOnInput(testNumbers, randomNumbers);
}


void testMixed() {
    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(-RANDOM_MAX,RANDOM_MAX);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

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

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        randomNumbers.push_back(randomValue/QUERY_RANDOM_DIV);
    }

    std::cout << std::endl << " ------ MIXED VALUES VECTOR ------ " << std::endl << std::endl;

    doTestsOnInput(testNumbers, randomNumbers);
}


void testProgressive() {
    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(-RANDOM_MAX,RANDOM_MAX);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Progressive reversed number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        testNumbers.push_back(INPUTSIZE-i);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        randomNumbers.push_back(randomValue/QUERY_RANDOM_DIV);
    }

    std::cout << std::endl << " ------ REVERSE SORTED VECTOR ------ " << std::endl << std::endl;

    doTestsOnInput(testNumbers, randomNumbers);
}



/* ----- FUNCTION IMPLEMENTATION ----- */


bool aabbOverlap(const Segment1D &segment1, const Segment1D &segment2) {
    double s1Min = std::min(segment1.getP1(), segment1.getP2());
    double s2Min = std::min(segment2.getP1(), segment2.getP2());
    double s1Max = std::max(segment1.getP1(), segment1.getP2());
    double s2Max = std::max(segment2.getP1(), segment2.getP2());
    return (s1Min <= s2Max && s1Max >= s2Min);
}

bool aabbOverlap(const Segment2D &segment1, const Segment2D &segment2) {
    double s1MinX = std::min(segment1.getP1().x(), segment1.getP2().x());
    double s2MinX = std::min(segment2.getP1().x(), segment2.getP2().x());
    double s1MaxX = std::max(segment1.getP1().x(), segment1.getP2().x());
    double s2MaxX = std::max(segment2.getP1().x(), segment2.getP2().x());

    double s1MinY = std::min(segment1.getP1().y(), segment1.getP2().y());
    double s2MinY = std::min(segment2.getP1().y(), segment2.getP2().y());
    double s1MaxY = std::max(segment1.getP1().y(), segment1.getP2().y());
    double s2MaxY = std::max(segment2.getP1().y(), segment2.getP2().y());

    return
            (s1MinX <= s2MaxX && s1MaxX >= s2MinX) &&
            (s1MinY <= s2MaxY && s1MaxY >= s2MinY);
}




double aabbValueExtractor(const Segment1D& segment, const AABBValueType& valueType, const int& dim) {
    if (valueType == AABBValueType::MIN) {
        switch (dim) {
        case 1:
            return (double) std::min(segment.getP1(), segment.getP2());
        }
    }
    if (valueType == AABBValueType::MAX) {
        switch (dim) {
        case 1:
            return (double) std::max(segment.getP1(), segment.getP2());
        }
    }

    assert(false);
    return false;
}

double aabbValueExtractor(const Segment2D& segment, const AABBValueType& valueType, const int& dim) {
    if (valueType == AABBValueType::MIN) {
        switch (dim) {
        case 1:
            return (double) std::min(segment.getP1().x(), segment.getP2().x());
        case 2:
            return (double) std::min(segment.getP1().y(), segment.getP2().y());
        }
    }
    if (valueType == AABBValueType::MAX) {
        switch (dim) {
        case 1:
            return (double) std::max(segment.getP1().x(), segment.getP2().x());
        case 2:
            return (double) std::max(segment.getP1().y(), segment.getP2().y());
        }
    }

    assert(false);
    return false;
}




bool segmentIntersection(const Segment1D &segment1, const Segment1D &segment2) {
    double s1Min = std::min(segment1.getP1(), segment1.getP2());
    double s2Min = std::min(segment2.getP1(), segment2.getP2());
    double s1Max = std::max(segment1.getP1(), segment1.getP2());
    double s2Max = std::max(segment2.getP1(), segment2.getP2());
    return (s1Min <= s2Max && s1Max >= s2Min);
}

bool segmentIntersection(const Segment2D &segment1, const Segment2D &segment2) {
    double s1MinX = std::min(segment1.getP1().x(), segment1.getP2().x());
    double s2MinX = std::min(segment2.getP1().x(), segment2.getP2().x());
    double s1MaxX = std::max(segment1.getP1().x(), segment1.getP2().x());
    double s2MaxX = std::max(segment2.getP1().x(), segment2.getP2().x());

    double s1MinY = std::min(segment1.getP1().y(), segment1.getP2().y());
    double s2MinY = std::min(segment2.getP1().y(), segment2.getP2().y());
    double s1MaxY = std::max(segment1.getP1().y(), segment1.getP2().y());
    double s2MaxY = std::max(segment2.getP1().y(), segment2.getP2().y());

    Point2D a(s1MinX, s1MinY);
    Point2D b(s1MaxX, s1MaxY);

    Point2D c(s2MinX, s2MinY);

    Point2D d(s2MaxX, s2MaxY);


    float den = ((d.y()-c.y())*(b.x()-a.x())-(d.x()-c.x())*(b.y()-a.y()));
    float num1 = ((d.x() - c.x())*(a.y()-c.y()) - (d.y()- c.y())*(a.x()-c.x()));
    float num2 = ((b.x()-a.x())*(a.y()-c.y())-(b.y()-a.y())*(a.x()-c.x()));
    float u1 = num1/den;
    float u2 = num2/den;

    /* The two lines are coincidents */
    if (den == 0 && num1 == 0 && num2 == 0)
        return true;
    /* The two lines are parallel */
    if (den == 0)
        return false;
    /* Lines do not collide */
    if (u1 < 0 || u1 > 1 || u2 < 0 || u2 > 1)
        return false;
    /* Lines DO collide */
    return true;
}




void printHeader() {
    std::cout <<
         std::setw(INDENTSPACE) << std::left << "STRUCTURE" <<
         std::setw(INDENTSPACE) << std::left << "CONSTR." <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "QUERY (C)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "OVQUERY (C)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "CHQUERY (C)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "ITERATION" <<
         std::setw(INDENTSPACE) << std::left << "CLEAR" <<
         std::setw(INDENTSPACE) << std::left << "INSERT" <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "RQUERY (D)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "ITERATION" <<
         std::setw(INDENTSPACE) << std::left << "ERASE" <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "RQUERY (E)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "TOTAL" <<
         std::endl << std::endl;
}





void doTestsOnInput(std::vector<int> &testNumbers, std::vector<int> &randomNumbers)
{
    std::vector<Segment1D> testSegment1D;
    std::vector<Segment1D> randomSegment1D;

    for (size_t i = 0; i < testNumbers.size(); i++) {
        Segment1D segment(testNumbers.at(i), testNumbers.at(testNumbers.size()-1-i));

        if (std::abs(segment.getP2() - segment.getP1()) > MAXLENGTH)
            segment.setP2(segment.getP1() + MAXLENGTH);

        testSegment1D.push_back(segment);
    }
    for (size_t i = 0; i < randomNumbers.size(); i++) {
        Segment1D segment(randomNumbers.at(i), randomNumbers.at(randomNumbers.size()-1-i));

        if (std::abs(segment.getP2() - segment.getP1()) > MAXLENGTH)
            segment.setP2(segment.getP1() + MAXLENGTH);

        randomSegment1D.push_back(segment);
    }

    std::vector<Segment2D> testSegment2D;
    std::vector<Segment2D> randomSegment2D;

    for (size_t i = 0; i < testNumbers.size()-1; i++) {
        Point2D p1(testNumbers.at(i), testNumbers.at(testNumbers.size()-1-i));
        Point2D p2;
        if (i < testNumbers.size()-1) {
            p2.set(testNumbers.at(i+1), testNumbers.at(testNumbers.size()-2-i));
        }
        else {
            p2.set(p1.x() + MAXLENGTH, p1.y() + MAXLENGTH);
        }

        if (std::abs(p2.x() - p1.x()) > MAXLENGTH)
            p2.setXCoord(p1.x() + MAXLENGTH);

        if (std::abs(p2.y() - p1.y()) > MAXLENGTH)
            p2.setYCoord(p1.y() + MAXLENGTH);

        Segment2D segment(p1,p2);

        testSegment2D.push_back(segment);
    }
    for (size_t i = 0; i < randomNumbers.size(); i++) {
        Point2D p1(randomNumbers.at(i), randomNumbers.at(randomNumbers.size()-1-i));
        Point2D p2;
        if (i < randomNumbers.size()-1) {
            p2.set(randomNumbers.at(i+1), randomNumbers.at(randomNumbers.size()-2-i));
        }
        else {
            p2.set(p1.x() + MAXLENGTH, p1.y() + MAXLENGTH);
        }

        if (std::abs(p2.x() - p1.x()) > MAXLENGTH)
            p2.setXCoord(p1.x() + MAXLENGTH);

        if (std::abs(p2.y() - p1.y()) > MAXLENGTH)
            p2.setYCoord(p1.y() + MAXLENGTH);

        Segment2D segment(p1,p2);

        randomSegment2D.push_back(segment);
    }

    printHeader();

    if (!ONLYEFFICIENT) {
        for (int t = 0; t < ITERATION; t++) {            
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BRUTE1D";
            testBrute1D(testSegment1D, randomSegment1D);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }
    }

    for (int t = 0; t < ITERATION; t++) {        
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "AABB1D";
        testAABBTree1D(testSegment1D, randomSegment1D);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }


    std::cout << std::endl;

    if (!ONLYEFFICIENT) {
        for (int t = 0; t < ITERATION; t++) {            
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BRUTE2D";
            testBrute2D(testSegment2D, randomSegment2D);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }
    }

    for (int t = 0; t < ITERATION; t++) {        
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "AABB2D";
        testAABBTree2D(testSegment2D, randomSegment2D);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }


    testSegment1D.clear();
    randomSegment1D.clear();

    testSegment2D.clear();
    randomSegment2D.clear();

    std::cout << std::endl;
}




void testBrute1D(std::vector<Segment1D>& testSegments, std::vector<Segment1D>& randomSegments) {

    typedef std::set<Segment1D>::iterator Iterator;


    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    std::set<Segment1D>* set = new std::set<Segment1D>(testSegments.begin(), testSegments.end());

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = set->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const Segment1D& segment : testSegments) {
        Iterator it = set->find(segment);
        bool found = (it != set->end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const Segment1D& segment : randomSegments) {
        Iterator it = set->find(segment);
        if (it != set->end()) {
            foundConstruction++;
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;



    /* Overlap query (construction) */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        std::vector<Segment1D> out;

        for (const Segment1D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        assert(out.size() >= 1);
    }

    size_t foundOverlapConstruction = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment1D& segment = randomSegments.at(i);
        std::vector<Segment1D> out;
        for (const Segment1D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        foundOverlapConstruction += out.size();
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapConstruction;




    /* Overlap check (construction) */

    size_t foundOverlapCheckConstruction = 0;
    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment1D& segment = randomSegments.at(i);

        for (const Segment1D& setSegment : *set) {
            if (segmentIntersection(segment, setSegment)) {
                foundOverlapCheckConstruction++;
                break;
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapCheckTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapCheckTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapCheckConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment1D& segment : *set) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    t2 = high_resolution_clock::now();

    auto constructionIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionIterationTime/1000;



    /* Clear */

    t1 = high_resolution_clock::now();

    set->clear();

    t2 = high_resolution_clock::now();

    assert(set->size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        set->insert(segment);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = set->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Overlap query */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        std::vector<Segment1D> out;

        for (const Segment1D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        assert(out.size() >= 1);
    }

    size_t foundOverlapInsert = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment1D& segment = randomSegments.at(i);

        std::vector<Segment1D> out;
        for (const Segment1D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        foundOverlapInsert += out.size();

    }

    t2 = high_resolution_clock::now();

    auto overlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) overlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment1D& segment : *set) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    t2 = high_resolution_clock::now();

    auto iterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) iterationTime/1000;




    /* Erase */


    t1 = high_resolution_clock::now();

    //Deleting second half of the vector
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment1D& number = testSegments.at(i);
        set->erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment1D& number = testSegments.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(set->find(number) == set->end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = set->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Overlap query (erase) */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        std::vector<Segment1D> out;

        for (const Segment1D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }
    }

    size_t foundOverlapErase = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment1D& segment = randomSegments.at(i);

        std::vector<Segment1D> out;
        for (const Segment1D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        foundOverlapErase += out.size();
    }

    t2 = high_resolution_clock::now();

    auto eraseOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseOverlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testSegments.size() && j < randomSegments.size(); i++, j++) {
        const Segment1D& testSegment = testSegments.at(i);
        const Segment1D& randomSegment = randomSegments.at(j);
        set->erase(testSegment);
        set->insert(randomSegment);
    }


    /* Clear */
    set->clear();

    assert(set->empty());

    Segment1D sampleSegment(1,1);

    assert(set->find(sampleSegment) == set->end());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    delete set;

    std::cout << std::endl;
}


void testAABBTree1D(std::vector<Segment1D>& testSegments, std::vector<Segment1D>& randomSegments) {
    AABBTree<1, Segment1D>* tree = new AABBTree<1, Segment1D>(&aabbValueExtractor);

    typedef AABBTree<1,Segment1D>::iterator Iterator;



    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    tree->construction(testSegments);

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = tree->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree->getHeight();



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const Segment1D& segment : testSegments) {
        Iterator it = tree->find(segment);
        bool found = (it != tree->end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const Segment1D& segment : randomSegments) {
        Iterator it = tree->find(segment);
        if (it != tree->end()) {
            foundConstruction++;
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Overlap query (construction) */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        std::vector<Iterator> out;

        tree->aabbOverlapQuery(segment, std::back_inserter(out));

        assert(out.size() >= 1);

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    size_t foundOverlapConstruction = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        std::vector<Iterator> out;

        Segment1D& segment = randomSegments.at(i);

        tree->aabbOverlapQuery(segment, std::back_inserter(out));
        foundOverlapConstruction += out.size();

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapConstruction;



    /* Overlap check (construction) */

    size_t foundOverlapCheckConstruction = 0;
    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment1D& segment = randomSegments.at(i);

        if (tree->aabbOverlapCheck(segment, &segmentIntersection)) {
            foundOverlapCheckConstruction++;
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapCheckTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapCheckTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapCheckConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment1D& segment : *tree) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    t2 = high_resolution_clock::now();

    auto constructionIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionIterationTime/1000;



    /* Clear */

    t1 = high_resolution_clock::now();

    tree->clear();

    t2 = high_resolution_clock::now();

    assert(tree->size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const Segment1D& testSegment : testSegments) {
        tree->insert(testSegment);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = tree->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree->getHeight();




    /* Overlap query */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        std::vector<Iterator> out;

        tree->aabbOverlapQuery(segment, std::back_inserter(out));

        assert(out.size() >= 1);

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    size_t foundOverlapInsert = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        std::vector<Iterator> out;

        Segment1D& segment = randomSegments.at(i);
        tree->aabbOverlapQuery(segment, std::back_inserter(out));
        foundOverlapInsert += out.size();

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    t2 = high_resolution_clock::now();

    auto overlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) overlapQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment1D& segment : *tree) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    t2 = high_resolution_clock::now();

    auto iterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) iterationTime/1000;




    /* Erase */


    t1 = high_resolution_clock::now();

    //Deleting second half of the vector
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment1D& segment = testSegments.at(i);
        tree->erase(segment);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment1D& segment = testSegments.at(i);
        CG3_SUPPRESS_WARNING(segment);
        assert(tree->find(segment) == tree->end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = tree->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree->getHeight();



    /* Overlap query (erase) */

    t1 = high_resolution_clock::now();

    for (const Segment1D& segment : testSegments) {
        std::vector<Iterator> out;

        tree->aabbOverlapQuery(segment, std::back_inserter(out));
    }

    size_t foundOverlapErase = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        std::vector<Iterator> out;

        Segment1D& segment = randomSegments.at(i);

        tree->aabbOverlapQuery(segment, std::back_inserter(out));
        foundOverlapErase += out.size();

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    t2 = high_resolution_clock::now();

    auto eraseOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseOverlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testSegments.size() && j < randomSegments.size(); i++, j++) {
        const Segment1D& testSegment = testSegments.at(i);
        const Segment1D& randomSegment = randomSegments.at(j);
        tree->erase(testSegment);
        tree->insert(randomSegment);
    }


    /* Clear */
    tree->clear();

    assert(tree->empty());

    Segment1D sampleSegment(1,1);

    std::vector<Iterator> out;
    tree->aabbOverlapQuery(sampleSegment, std::back_inserter(out));
    assert(out.size() == 0);


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;



    delete tree;

    std::cout << std::endl;
}





void testBrute2D(std::vector<Segment2D>& testSegments, std::vector<Segment2D>& randomSegments) {

    typedef std::set<Segment2D>::iterator Iterator;


    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    std::set<Segment2D>* set = new std::set<Segment2D>(testSegments.begin(), testSegments.end());

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = set->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const Segment2D& segment : testSegments) {
        Iterator it = set->find(segment);
        bool found = (it != set->end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const Segment2D& segment : randomSegments) {
        Iterator it = set->find(segment);
        if (it != set->end()) {
            foundConstruction++;
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;



    /* Overlap query (construction) */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        std::vector<Segment2D> out;

        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        assert(out.size() >= 1);
    }

    size_t foundOverlapConstruction = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment2D& segment = randomSegments.at(i);
        std::vector<Segment2D> out;
        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        foundOverlapConstruction += out.size();
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapQueryTime/1000;


    for (const Segment2D& segment : testSegments) {
        std::vector<Segment2D> out;

        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        assert(out.size() >= 1);
    }


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapConstruction;




    /* Overlap check (construction) */

    size_t foundOverlapCheckConstruction = 0;
    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment2D& segment = randomSegments.at(i);

        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment) && segmentIntersection(segment, setSegment)) {
                foundOverlapCheckConstruction++;
                break;
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapCheckTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapCheckTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapCheckConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment2D& segment : *set) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    t2 = high_resolution_clock::now();

    auto constructionIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionIterationTime/1000;



    /* Clear */

    t1 = high_resolution_clock::now();

    set->clear();

    t2 = high_resolution_clock::now();

    assert(set->size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        set->insert(segment);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = set->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Overlap query */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        std::vector<Segment2D> out;

        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        assert(out.size() >= 1);
    }

    size_t foundOverlapInsert = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment2D& segment = randomSegments.at(i);

        std::vector<Segment2D> out;
        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        foundOverlapInsert += out.size();

    }

    t2 = high_resolution_clock::now();

    auto overlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) overlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment2D& segment : *set) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    t2 = high_resolution_clock::now();

    auto iterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) iterationTime/1000;




    /* Erase */


    t1 = high_resolution_clock::now();

    //Deleting second half of the vector
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment2D& number = testSegments.at(i);
        set->erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment2D& number = testSegments.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(set->find(number) == set->end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = set->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Overlap query (erase) */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        std::vector<Segment2D> out;

        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }
    }

    size_t foundOverlapErase = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment2D& segment = randomSegments.at(i);

        std::vector<Segment2D> out;
        for (const Segment2D& setSegment : *set) {
            if (aabbOverlap(segment, setSegment)) {
                out.push_back(setSegment);
            }
        }

        foundOverlapErase += out.size();
    }

    t2 = high_resolution_clock::now();

    auto eraseOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseOverlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testSegments.size() && j < randomSegments.size(); i++, j++) {
        const Segment2D& testSegment = testSegments.at(i);
        const Segment2D& randomSegment = randomSegments.at(j);
        set->erase(testSegment);
        set->insert(randomSegment);
    }


    /* Clear */
    set->clear();

    assert(set->empty());

    Point2D p1(1,1);
    Point2D p2(1,1);
    Segment2D sampleSegment(p1,p2);

    assert(set->find(sampleSegment) == set->end());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    delete set;

    std::cout << std::endl;
}



void testAABBTree2D(std::vector<Segment2D>& testSegments, std::vector<Segment2D>& randomSegments) {

    AABBTree<2, Segment2D>* tree = new AABBTree<2, Segment2D>(&aabbValueExtractor);

    typedef AABBTree<2,Segment2D>::iterator Iterator;



    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    tree->construction(testSegments);

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = tree->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree->getHeight();



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const Segment2D& segment : testSegments) {
        Iterator it = tree->find(segment);
        bool found = (it != tree->end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const Segment2D& segment : randomSegments) {
        Iterator it = tree->find(segment);
        if (it != tree->end()) {
            foundConstruction++;
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Overlap query (construction) */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        std::vector<Iterator> out;

        tree->aabbOverlapQuery(segment, std::back_inserter(out));

        assert(out.size() >= 1);

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    size_t foundOverlapConstruction = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        std::vector<Iterator> out;

        Segment2D& segment = randomSegments.at(i);

        tree->aabbOverlapQuery(segment, std::back_inserter(out));
        foundOverlapConstruction += out.size();

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapConstruction;





    /* Overlap check (construction) */

    size_t foundOverlapCheckConstruction = 0;
    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        Segment2D& segment = randomSegments.at(i);

        if (tree->aabbOverlapCheck(segment, &segmentIntersection)) {
            foundOverlapCheckConstruction++;
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionOverlapCheckTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionOverlapCheckTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapCheckConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment2D& segment : *tree) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    t2 = high_resolution_clock::now();

    auto constructionIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionIterationTime/1000;



    /* Clear */

    t1 = high_resolution_clock::now();

    tree->clear();

    t2 = high_resolution_clock::now();

    assert(tree->size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const Segment2D& testSegment : testSegments) {
        tree->insert(testSegment);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = tree->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree->getHeight();




    /* Overlap query */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        std::vector<Iterator> out;

        tree->aabbOverlapQuery(segment, std::back_inserter(out));

        assert(out.size() >= 1);

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    size_t foundOverlapInsert = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        std::vector<Iterator> out;

        Segment2D& segment = randomSegments.at(i);
        tree->aabbOverlapQuery(segment, std::back_inserter(out));
        foundOverlapInsert += out.size();

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    t2 = high_resolution_clock::now();

    auto overlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) overlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapInsert;





    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Segment2D& segment : *tree) {
            CG3_SUPPRESS_WARNING(segment);
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    t2 = high_resolution_clock::now();

    auto iterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) iterationTime/1000;




    /* Erase */


    t1 = high_resolution_clock::now();

    //Deleting second half of the vector
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment2D& segment = testSegments.at(i);
        tree->erase(segment);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testSegments.size()/2; i < testSegments.size(); i++) {
        const Segment2D& segment = testSegments.at(i);
        CG3_SUPPRESS_WARNING(segment);
        assert(tree->find(segment) == tree->end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = tree->size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree->getHeight();



    /* Overlap query (erase) */

    t1 = high_resolution_clock::now();

    for (const Segment2D& segment : testSegments) {
        std::vector<Iterator> out;

        tree->aabbOverlapQuery(segment, std::back_inserter(out));
    }

    size_t foundOverlapErase = 0;
    for (size_t i = 0; i < randomSegments.size()-1; i++) {
        std::vector<Iterator> out;

        Segment2D& segment = randomSegments.at(i);

        tree->aabbOverlapQuery(segment, std::back_inserter(out));
        foundOverlapErase += out.size();

        for (Iterator outIt : out) {
            CG3_SUPPRESS_WARNING(outIt);
            assert(aabbOverlap(*outIt, segment));
        }
    }

    t2 = high_resolution_clock::now();

    auto eraseOverlapQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseOverlapQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundOverlapErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testSegments.size() && j < randomSegments.size(); i++, j++) {
        const Segment2D& testSegment = testSegments.at(i);
        const Segment2D& randomSegment = randomSegments.at(j);
        tree->erase(testSegment);
        tree->insert(randomSegment);
    }


    /* Clear */
    tree->clear();

    assert(tree->empty());

    Point2D p1(1,1);
    Point2D p2(1,1);
    Segment2D sampleSegment(p1,p2);

    std::vector<Iterator> out;
    tree->aabbOverlapQuery(sampleSegment, std::back_inserter(out));
    assert(out.size() == 0);


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;



    delete tree;

    std::cout << std::endl;
}



}
