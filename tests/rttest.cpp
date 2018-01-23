/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "rttest.h"

#include <iostream>
#include <random>
#include <chrono>
#include <assert.h>
#include <iomanip>

#include <set>
#include <vector>

#include "cg3/geometry/2d/point2d.h"

#include "cg3/data_structures/trees/bstinner.h"
#include "cg3/data_structures/trees/avlinner.h"
#include "cg3/data_structures/trees/bstleaf.h"
#include "cg3/data_structures/trees/avlleaf.h"

#include "cg3/data_structures/trees/rangetree.h"

#define ITERATION 1
#define INDENTSPACE 12

#define INPUTSIZE 10000
#define RANDOM_MAX (INPUTSIZE*100)
#define QUERY_RANDOM_DIV 10
#define ONLYEFFICIENT (INPUTSIZE > 20000)

namespace RTTests {


/* ----- TYPEDEFS ----- */

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef high_resolution_clock::time_point time_point;


template <class T> using BSTLeaf = typename cg3::BSTLeaf<T>;
template <class T> using AVLLeaf = typename cg3::AVLLeaf<T>;

template <class T> using BSTInner = typename cg3::BSTInner<T>;
template <class T> using AVLInner = typename cg3::AVLInner<T>;

template <class T> using RangeTree = typename cg3::RangeTree<T>;

typedef cg3::Point2D<int> Point2D;


/* ----- FUNCTION DECLARATION ----- */


bool intComparator(
        const int& o1,
        const int& o2);

bool point2DDimensionComparatorX(
        const Point2D &o1,
        const Point2D &o2);

bool point2DDimensionComparatorY(
        const Point2D &o1,
        const Point2D &o2);

bool point2DComparator(const Point2D& o1, const Point2D& o2);

void printHeader();

void doTestsOnInput(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);

void testBrute1D(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);
template <class B>
void testBST(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);
void testRangeTree1D(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);

void testBrute2D(std::vector<Point2D>& testNumbers, std::vector<Point2D>& randomNumbers);
void testRangeTree2D(std::vector<Point2D>& testNumbers, std::vector<Point2D>& randomNumbers);



/* ----- IMPLEMENTATION ----- */

void testCorrectness() {
    std::vector<RangeTree<Point2D>::LessComparator> customComparators;
    customComparators.push_back(&point2DDimensionComparatorX);
    customComparators.push_back(&point2DDimensionComparatorY);

    RangeTree<Point2D> tree1(2, customComparators);
    RangeTree<Point2D> tree2(2, customComparators);

    tree1.insert(Point2D(1,2));
    tree1.insert(Point2D(3,54));
    tree1.insert(Point2D(19,10));
    tree1.insert(Point2D(43,2));

    //Check copy constructor and move constructor
    tree2 = tree1;
    tree2.clear();

    std::vector<Point2D> vec;
    for (int i = 0; i < 2000; i+=2)
        vec.push_back(Point2D(i,i+1));

    tree2 = RangeTree<Point2D>(2, vec, customComparators);
    tree2.clear();

    RangeTree<Point2D> tree3(
                std::move(
                    RangeTree<Point2D>(2, vec, customComparators)));
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

bool intComparator(
        const int& o1,
        const int& o2)
{
    return o1 < o2;
}

bool point2DDimensionComparatorX(
        const Point2D &o1,
        const Point2D &o2)
{
    if (o1.x() < o2.x())
        return true;
    if (o2.x() < o1.x())
        return false;
    return point2DComparator(o1,o2);
}

bool point2DDimensionComparatorY(
        const Point2D &o1,
        const Point2D &o2)
{
    if (o1.y() < o2.y())
        return true;
    if (o2.y() < o1.y())
        return false;
    return point2DComparator(o1,o2);
}

bool point2DComparator(const Point2D& o1, const Point2D& o2) {
    if (o1.x() < o2.x())
        return true;
    if (o2.x() < o1.x())
        return false;
    return o1.y() < o2.y();
}


void printHeader() {
    std::cout <<
         std::setw(INDENTSPACE) << std::left << "STRUCTURE" <<
         std::setw(INDENTSPACE) << std::left << "CONSTR." <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "QUERY (C)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "RQUERY (C)" <<
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
    std::vector<Point2D> testPoints;
    std::vector<Point2D> randomPoints;

    for (size_t i = 0; i < testNumbers.size(); i++) {
        Point2D p(testNumbers.at(i), testNumbers.at(testNumbers.size()-1-i));
        testPoints.push_back(p);
    }
    for (size_t i = 0; i < randomNumbers.size(); i++) {
        Point2D p(randomNumbers.at(i), randomNumbers.at(randomNumbers.size()-1-i));
        randomPoints.push_back(p);
    }

    printHeader();

    if (!ONLYEFFICIENT) {
        for (int t = 0; t < ITERATION; t++) {
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BRUTE1D";
            testBrute1D(testNumbers, randomNumbers);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }
    }


    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "AVL (I) 1D";
        testBST<AVLInner<int>>(testNumbers, randomNumbers);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }


    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "AVL (L) 1D";
        testBST<AVLLeaf<int>>(testNumbers, randomNumbers);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }


    if (!ONLYEFFICIENT) {
        for (int t = 0; t < ITERATION; t++) {
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BST (I) 1D";
            testBST<BSTInner<int>>(testNumbers, randomNumbers);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }



        for (int t = 0; t < ITERATION; t++) {
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BST (L) 1D";
            testBST<BSTLeaf<int>>(testNumbers, randomNumbers);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }
    }


    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "RT 1D";
        testRangeTree1D(testNumbers, randomNumbers);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }


    std::cout << std::endl;

    if (!ONLYEFFICIENT) {
        for (int t = 0; t < ITERATION; t++) {
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BRUTE2D";
            testBrute2D(testPoints, randomPoints);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }
    }


    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "RT 2D";
        testRangeTree2D(testPoints, randomPoints);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }


    testNumbers.clear();
    randomNumbers.clear();

    testPoints.clear();
    randomPoints.clear();

    std::cout << std::endl;
}




void testBrute1D(std::vector<int>& testNumbers, std::vector<int>& randomNumbers) {

    typedef std::set<int>::iterator Iterator;


    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    std::set<int> set(testNumbers.begin(), testNumbers.end());

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const int& number : testNumbers) {
        Iterator it = set.find(number);
        bool found = (it != set.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = set.find(number);
        if (it != set.end()) {
            foundConstruction++;
            assert(*it == number);
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;



    /* Range query (construction) */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<int> out;

        for (const int& setNumber : set) {
            if (number <= setNumber && setNumber <= number) {
                out.push_back(setNumber);
            }
        }

        assert(out.size() == 1);
    }

    size_t foundRangeConstruction = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            std::vector<int> out;
            for (const int& setNumber : set) {
                if (num1 <= setNumber && setNumber <= num2) {
                    out.push_back(setNumber);
                }
            }

            foundRangeConstruction += out.size();

            for (const int& outNumber : out) {
                assert(num1 <= outNumber && outNumber <= num2);
                CG3_SUPPRESS_WARNING(outNumber);
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionrangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionrangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : set) {
            assert(lastNumber < number);
            lastNumber = number;
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

    set.clear();

    t2 = high_resolution_clock::now();

    assert(set.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const int& testNumber : testNumbers) {
        set.insert(testNumber);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Range query */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<int> out;

        for (const int& setNumber : set) {
            if (number <= setNumber && setNumber <= number) {
                out.push_back(setNumber);
            }
        }

        assert(out.size() == 1);
    }

    size_t foundRangeInsert = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            std::vector<int> out;
            for (const int& setNumber : set) {
                if (num1 <= setNumber && setNumber <= num2) {
                    out.push_back(setNumber);
                }
            }

            foundRangeInsert += out.size();

            for (const int& outNumber : out) {
                assert(num1 <= outNumber && outNumber <= num2);
                CG3_SUPPRESS_WARNING(outNumber);
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto rangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) rangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : set) {
            assert(lastNumber < number);
            lastNumber = number;
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
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        set.erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {        
        const int& number = testNumbers.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(set.find(number) == set.end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Range query (erase) */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<int> out;

        for (const int& setNumber : set) {
            if (number <= setNumber && setNumber <= number) {
                out.push_back(setNumber);
            }
        }
    }

    size_t foundRangeErase = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            std::vector<int> out;
            for (const int& setNumber : set) {
                if (num1 <= setNumber && setNumber <= num2) {
                    out.push_back(setNumber);
                }
            }

            foundRangeErase += out.size();


            for (const int& outNumber : out) {
                CG3_SUPPRESS_WARNING(outNumber);
                assert(num1 <= outNumber && outNumber <= num2);
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto eraserangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraserangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testNumbers.size() && j < randomNumbers.size(); i++, j++) {
        const int& testNumber = testNumbers.at(i);
        const int& randomNumber = testNumbers.at(j);
        set.erase(testNumber);
        set.insert(randomNumber);
    }


    /* Clear */
    set.clear();

    assert(set.empty());

    assert(set.find(5) == set.end());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    std::cout << std::endl;
}


template <class B>
void testBST(std::vector<int>& testNumbers, std::vector<int>& randomNumbers) {
    B tree;

    typedef typename B::iterator Iterator;



    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    tree.construction(testNumbers);

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const int& number : testNumbers) {
        Iterator it = tree.find(number);
        bool found = (it != tree.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = tree.find(number);
        if (it != tree.end()) {
            foundConstruction++;
            assert(*it == number);
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;



    /* Range query (construction) */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<Iterator> out;

        tree.rangeQuery(number, number, std::back_inserter(out));

        assert(out.size() == 1);
    }

    size_t foundRangeConstruction = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        std::vector<Iterator> out;

        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            tree.rangeQuery(num1, num2, std::back_inserter(out));
            foundRangeConstruction += out.size();

            for (Iterator outIt : out)
                assert(num1 <= *outIt && *outIt <= num2);
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionrangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionrangeQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;        
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
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

    tree.clear();

    t2 = high_resolution_clock::now();

    assert(tree.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const int& testNumber : testNumbers) {
        tree.insert(testNumber);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Range query */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<Iterator> out;

        tree.rangeQuery(number, number, std::back_inserter(out));

        assert(out.size() == 1);
    }

    size_t foundRangeInsert = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        std::vector<Iterator> out;

        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            tree.rangeQuery(num1, num2, std::back_inserter(out));
            foundRangeInsert += out.size();

            for (Iterator outIt : out)
                assert(num1 <= *outIt && *outIt <= num2);
        }
    }

    t2 = high_resolution_clock::now();

    auto rangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) rangeQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;        
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
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
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        tree.erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(tree.find(number) == tree.end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Range query (erase) */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<Iterator> out;

        tree.rangeQuery(number, number, std::back_inserter(out));
    }

    size_t foundRangeErase = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        std::vector<Iterator> out;

        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            tree.rangeQuery(num1, num2, std::back_inserter(out));
            foundRangeErase += out.size();

            for (Iterator outIt : out)
                assert(num1 <= *outIt && *outIt <= num2);
        }
    }

    t2 = high_resolution_clock::now();

    auto eraserangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraserangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testNumbers.size() && j < randomNumbers.size(); i++, j++) {
        const int& testNumber = testNumbers.at(i);
        const int& randomNumber = testNumbers.at(j);
        tree.erase(testNumber);
        tree.insert(randomNumber);
    }


    /* Clear */

    tree.clear();


    assert(tree.empty());

    std::vector<Iterator> out;
    tree.rangeQuery(-RANDOM_MAX, RANDOM_MAX, std::back_inserter(out));
    assert(out.size() == 0);


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    std::cout << std::endl;
}


void testRangeTree1D(std::vector<int>& testNumbers, std::vector<int>& randomNumbers) {
    std::vector<RangeTree<int>::LessComparator> customComparators;
    customComparators.push_back(&intComparator);

    RangeTree<int> tree(1, customComparators);

    typedef RangeTree<int>::iterator Iterator;



    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    tree.construction(testNumbers);

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const int& number : testNumbers) {
        Iterator it = tree.find(number);
        bool found = (it != tree.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = tree.find(number);
        if (it != tree.end()) {
            foundConstruction++;
            assert(*it == number);
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Range query (construction) */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<Iterator> out;

        tree.rangeQuery(number, number, std::back_inserter(out));

        assert(out.size() == 1);
    }

    size_t foundRangeConstruction = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        std::vector<Iterator> out;

        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            tree.rangeQuery(num1, num2, std::back_inserter(out));
            foundRangeConstruction += out.size();

            for (Iterator outIt : out) {
                CG3_SUPPRESS_WARNING(outIt);
                assert(num1 <= *outIt && *outIt <= num2);
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionrangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionrangeQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
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

    tree.clear();

    t2 = high_resolution_clock::now();

    assert(tree.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const int& testNumber : testNumbers) {
        tree.insert(testNumber);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Range query */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<Iterator> out;

        tree.rangeQuery(number, number, std::back_inserter(out));

        assert(out.size() == 1);
    }

    size_t foundRangeInsert = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        std::vector<Iterator> out;

        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            tree.rangeQuery(num1, num2, std::back_inserter(out));
            foundRangeInsert += out.size();

            for (Iterator outIt : out) {
                CG3_SUPPRESS_WARNING(outIt);
                assert(num1 <= *outIt && *outIt <= num2);
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto rangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) rangeQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
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
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        tree.erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(tree.find(number) == tree.end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Range query (erase) */

    t1 = high_resolution_clock::now();

    for (const int& number : testNumbers) {
        std::vector<Iterator> out;

        tree.rangeQuery(number, number, std::back_inserter(out));
    }

    size_t foundRangeErase = 0;
    for (size_t i = 0; i < randomNumbers.size()-1; i += 2) {
        std::vector<Iterator> out;

        int& num1 = randomNumbers.at(i);
        int& num2 = randomNumbers.at(i+1);

        if (num1 <= num2) {
            tree.rangeQuery(num1, num2, std::back_inserter(out));
            foundRangeErase += out.size();

            for (Iterator outIt : out) {
                CG3_SUPPRESS_WARNING(outIt);
                assert(num1 <= *outIt && *outIt <= num2);
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto eraserangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraserangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testNumbers.size() && j < randomNumbers.size(); i++, j++) {
        const int& testNumber = testNumbers.at(i);
        const int& randomNumber = testNumbers.at(j);
        tree.erase(testNumber);
        tree.insert(randomNumber);
    }


    /* Clear */
    tree.clear();

    assert(tree.empty());

    std::vector<Iterator> out;
    tree.rangeQuery(-RANDOM_MAX, RANDOM_MAX, std::back_inserter(out));
    assert(out.size() == 0);


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;



    std::cout << std::endl;
}




void testBrute2D(std::vector<Point2D>& testPoints, std::vector<Point2D>& randomPoints) {

    typedef std::set<Point2D>::iterator Iterator;


    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    std::set<Point2D> set(testPoints.begin(), testPoints.end());

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";


    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const Point2D& point : testPoints) {
        Iterator it = set.find(point);
        bool found = (it != set.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const Point2D& point : randomPoints) {
        Iterator it = set.find(point);
        if (it != set.end()) {
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



    /* Range query (construction) */

    t1 = high_resolution_clock::now();

    for (const Point2D& point : testPoints) {
        std::vector<Point2D> out;

        for (const Point2D& setPoint : set) {
            if (point.x() <= setPoint.x() && setPoint.x() <= point.x()) {
                if (point.y() <= setPoint.y() && setPoint.y() <= point.y()) {
                    out.push_back(setPoint);
                }
            }
        }

        assert(out.size() == 1);
    }

    size_t foundRangeConstruction = 0;
    for (size_t i = 0; i < randomPoints.size()-1; i += 2) {
        Point2D& p1 = randomPoints.at(i);
        Point2D& p2 = randomPoints.at(i+1);

        if (p1.x() <= p2.x() && p1.y() <= p2.y()) {
            std::vector<Point2D> out;
            for (const Point2D& setPoint : set) {
                if (p1.x() <= setPoint.x() && setPoint.x() <= p2.x()) {
                    if (p1.y() <= setPoint.y() && setPoint.y() <= p2.y()) {
                        out.push_back(setPoint);
                    }
                }
            }

            foundRangeConstruction += out.size();

            for (const Point2D& outPoint : out) {
                CG3_SUPPRESS_WARNING(outPoint);
                assert(p1.x() <= outPoint.x() && outPoint.x() <= p2.x() &&
                       p1.y() <= outPoint.y() && outPoint.y() <= p2.y());
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionrangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionrangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Point2D& p : set) {
            CG3_SUPPRESS_WARNING(p);
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

    set.clear();

    t2 = high_resolution_clock::now();

    assert(set.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const Point2D& testPoint : testPoints) {
        set.insert(testPoint);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Range query */

    t1 = high_resolution_clock::now();

    for (const Point2D& point : testPoints) {
        std::vector<Point2D> out;

        for (const Point2D& setPoint : set) {
            if (point.x() <= setPoint.x() && setPoint.x() <= point.x()) {
                if (point.y() <= setPoint.y() && setPoint.y() <= point.y()) {
                    out.push_back(setPoint);
                }
            }
        }

        assert(out.size() == 1);
    }

    size_t foundRangeInsert = 0;
    for (size_t i = 0; i < randomPoints.size()-1; i += 2) {
        Point2D& p1 = randomPoints.at(i);
        Point2D& p2 = randomPoints.at(i+1);

        if (p1.x() <= p2.x() && p1.y() <= p2.y()) {
            std::vector<Point2D> out;
            for (const Point2D& setPoint : set) {
                if (p1.x() <= setPoint.x() && setPoint.x() <= p2.x()) {
                    if (p1.y() <= setPoint.y() && setPoint.y() <= p2.y()) {
                        out.push_back(setPoint);
                    }
                }
            }

            foundRangeInsert += out.size();

            for (const Point2D& outPoint : out) {
                CG3_SUPPRESS_WARNING(outPoint);
                assert(p1.x() <= outPoint.x() && outPoint.x() <= p2.x() &&
                       p1.y() <= outPoint.y() && outPoint.y() <= p2.y());
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto rangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) rangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
    {
        size_t numOfEntries = 0;
        for (const Point2D& point : set) {
            CG3_SUPPRESS_WARNING(point);
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
    for (size_t i = testPoints.size()/2; i < testPoints.size(); i++) {
        const Point2D& point = testPoints.at(i);
        set.erase(point);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testPoints.size()/2; i < testPoints.size(); i++) {
        const Point2D& point = testPoints.at(i);
        CG3_SUPPRESS_WARNING(point);
        assert(set.find(point) == set.end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Range query (erase) */

    t1 = high_resolution_clock::now();

    for (const Point2D& point : testPoints) {
        std::vector<Point2D> out;

        for (const Point2D& setPoint : set) {
            if (point.x() <= setPoint.x() && setPoint.x() <= point.x()) {
                if (point.y() <= setPoint.y() && setPoint.y() <= point.y()) {
                    out.push_back(setPoint);
                }
            }
        }
    }

    size_t foundRangeErase = 0;
    for (size_t i = 0; i < randomPoints.size()-1; i += 2) {
        Point2D& p1 = randomPoints.at(i);
        Point2D& p2 = randomPoints.at(i+1);

       if (p1.x() <= p2.x() && p1.y() <= p2.y()) {
            std::vector<Point2D> out;
            for (const Point2D& setNumber : set) {
                if (p1.x() <= setNumber.x() && setNumber.x() <= p2.x()) {
                    if (p1.y() <= setNumber.y() && setNumber.y() <= p2.y()) {
                        out.push_back(setNumber);
                    }
                }
            }

            foundRangeErase += out.size();

            for (const Point2D& outNumber : out) {
                CG3_SUPPRESS_WARNING(outNumber);
                assert(p1.x() <= outNumber.x() && outNumber.x() <= p2.x() &&
                       p1.y() <= outNumber.y() && outNumber.y() <= p2.y());
            }
        }
    }
    t2 = high_resolution_clock::now();

    auto eraserangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraserangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testPoints.size() && j < randomPoints.size(); i++, j++) {
        const Point2D& testPoint = testPoints.at(i);
        const Point2D& randomPoint = testPoints.at(j);
        set.erase(testPoint);
        set.insert(randomPoint);
    }

    /* Clear */

    set.clear();


    assert(set.empty());

    Point2D p(5,3);
    assert(set.find(p) == set.end());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    std::cout << std::endl;
}


void testRangeTree2D(std::vector<Point2D>& testPoints, std::vector<Point2D>& randomPoints) {
    std::vector<RangeTree<Point2D>::LessComparator> customComparators;
    customComparators.push_back(&point2DDimensionComparatorX);
    customComparators.push_back(&point2DDimensionComparatorY);

    RangeTree<Point2D> tree(2, customComparators);

    typedef RangeTree<Point2D>::iterator Iterator;



    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    tree.construction(testPoints);

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const Point2D& point : testPoints) {
        Iterator it = tree.find(point);
        bool found = (it != tree.end());

        if (found)
            foundConstruction++;


        assert(found);
    }

    for (const Point2D& point : randomPoints) {
        Iterator it = tree.find(point);
        if (it != tree.end()) {
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



    /* Range query (construction) */

    t1 = high_resolution_clock::now();

    for (const Point2D& point : testPoints) {
        std::vector<Iterator> out;

        tree.rangeQuery(point, point, std::back_inserter(out));

        assert(out.size() == 1);
    }

    size_t foundRangeConstruction = 0;
    for (size_t i = 0; i < randomPoints.size()-1; i += 2) {
        std::vector<Iterator> out;

        Point2D& p1 = randomPoints.at(i);
        Point2D& p2 = randomPoints.at(i+1);

        if (p1.x() <= p2.x() && p1.y() <= p2.y()) {
            tree.rangeQuery(p1, p2, std::back_inserter(out));
            foundRangeConstruction += out.size();

            for (Iterator outIt : out) {
                CG3_SUPPRESS_WARNING(outIt);
                assert(p1.x() <= (*outIt).x() && (*outIt).x() <= p2.x() &&
                       p1.y() <= (*outIt).y() && (*outIt).y() <= p2.y());
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionrangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionrangeQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeConstruction;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Point2D& point : tree) {
            CG3_SUPPRESS_WARNING(point);
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

    tree.clear();

    t2 = high_resolution_clock::now();

    assert(tree.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const Point2D& testPoint : testPoints) {
        tree.insert(testPoint);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Range query */

    t1 = high_resolution_clock::now();

    for (const Point2D& point : testPoints) {
        std::vector<Iterator> out;

        tree.rangeQuery(point, point, std::back_inserter(out));

        assert(out.size() == 1);
    }

    size_t foundRangeInsert = 0;
    for (size_t i = 0; i < randomPoints.size()-1; i += 2) {
        std::vector<Iterator> out;

        Point2D& p1 = randomPoints.at(i);
        Point2D& p2 = randomPoints.at(i+1);

        if (p1.x() <= p2.x() && p1.y() <= p2.y()) {
            tree.rangeQuery(p1, p2, std::back_inserter(out));
            foundRangeInsert += out.size();

            for (Iterator outIt : out) {
                CG3_SUPPRESS_WARNING(outIt);
                assert(p1.x() <= (*outIt).x() && (*outIt).x() <= p2.x() &&
                       p1.y() <= (*outIt).y() && (*outIt).y() <= p2.y());
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto rangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) rangeQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
        {
        size_t numOfEntries = 0;
        for (const Point2D& point : tree) {
            CG3_SUPPRESS_WARNING(point);
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
    for (size_t i = testPoints.size()/2; i < testPoints.size(); i++) {
        const Point2D& point = testPoints.at(i);
        tree.erase(point);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testPoints.size()/2; i < testPoints.size(); i++) {
        const Point2D& point = testPoints.at(i);
        CG3_SUPPRESS_WARNING(point);
        assert(tree.find(point) == tree.end());
    }



    /* Number of elements */

    size_t numberOfElementsErase = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Range query (erase) */

    t1 = high_resolution_clock::now();

    for (const Point2D& point : testPoints) {
        std::vector<Iterator> out;

        tree.rangeQuery(point, point, std::back_inserter(out));
    }

    size_t foundRangeErase = 0;
    for (size_t i = 0; i < randomPoints.size()-1; i += 2) {
        std::vector<Iterator> out;

        Point2D& p1 = randomPoints.at(i);
        Point2D& p2 = randomPoints.at(i+1);

        if (p1.x() <= p2.x() && p1.y() <= p2.y()) {
            tree.rangeQuery(p1, p2, std::back_inserter(out));
            foundRangeErase += out.size();

            for (Iterator outIt : out) {
                CG3_SUPPRESS_WARNING(outIt);
                assert(p1.x() <= (*outIt).x() && (*outIt).x() <= p2.x() &&
                       p1.y() <= (*outIt).y() && (*outIt).y() <= p2.y());
            }
        }
    }

    t2 = high_resolution_clock::now();

    auto eraserangeQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraserangeQueryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundRangeErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testPoints.size() && j < randomPoints.size(); i++, j++) {
        const Point2D& testPoint = testPoints.at(i);
        const Point2D& randomPoint = testPoints.at(j);
        tree.erase(testPoint);
        tree.insert(randomPoint);
    }



    /* Clear */

    tree.clear();


    assert(tree.empty());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;


    std::cout << std::endl;
}

}

