/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "rtsample.h"

#include <iostream>
#include <sstream>
#include <set>

#include <cg3/data_structures/trees/rangetree.h>

#include <cg3/geometry/2d/point2d.h>

typedef cg3::Point2Dd Point2Dd;


/* ---- COMPARATORS FOR POINTERS ----- */

bool point2DPointerDimensionComparatorX(Point2Dd* const& o1, Point2Dd* const& o2);
bool point2DPointerDimensionComparatorY(Point2Dd* const& o1, Point2Dd* const& o2);


/* ---- SAMPLES ----- */

void RTSample::execute()
{
    //Defining an alias for range tree for 2-dimensional points
    typedef cg3::RangeTree2D RangeTree2D;


    //Note that there is also a range tree for 3D points: cg3::RangeTree3D


    /* ----- BASIC USAGE ----- */


    //Creating vector for construction with object [12,15.9], [2,10.1], [45,65.0]
    std::cout << "Construction objects: [12,15.9], [2,10.1], [45,65.0]" << std::endl;
    std::vector<Point2Dd> pointVec;
    pointVec.push_back(Point2Dd(12,15.9));
    pointVec.push_back(Point2Dd(2,10.1));
    pointVec.push_back(Point2Dd(45,65.0));

    //Creating 2-dimensional-range-tree (for Point2D)
    //Point vector is optional
    std::cout << "Creating range trees with comparators..." << std::endl;
    RangeTree2D rangeTree2D(pointVec); //Vector of comparators

    //Insert objects: [100,52.25], [45,20], [3,25.0]
    std::cout << "Inserting objects: [100,52.25], [45,20], [3,25.0]" << std::endl;

    rangeTree2D.insert(Point2Dd(100,52.25));
    rangeTree2D.insert(Point2Dd(45,20));
    rangeTree2D.insert(Point2Dd(3,25.0));

    //Iteration
    std::cout << "The range tree contains:" << std::endl << "    ";
    for (const Point2Dd& point : rangeTree2D)
        std::cout << point << " ";
    std::cout << std::endl;

    //Find object [2,10.1]
    RangeTree2D::iterator queryIterator = rangeTree2D.find(Point2Dd(2,10.1));
    if (queryIterator != rangeTree2D.end())
        std::cout << "Object [2,10.1] is in the range tree!" << std::endl;
    else
        std::cout << "Object [2,10.1] is NOT in the range tree!" << std::endl;

    //Erase
    std::cout << "Erasing object [2,10.1]..." << std::endl;
    rangeTree2D.erase(Point2Dd(2,10.1));

    //Range query for the interval [3 - 99, 15.99 - 65.0]
    std::cout << "Range query for the interval [3 - 99, 15.99 - 65.0]" << std::endl << "    ";
    std::vector<RangeTree2D::iterator> rangeQueryResults;
    rangeTree2D.rangeQuery(
                Point2Dd(3,15.99),
                Point2Dd(99,65.0),
                std::back_inserter(rangeQueryResults));

    for (RangeTree2D::iterator it : rangeQueryResults) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    //Iteration with explicit iterators
    std::cout << "The range tree contains:" << std::endl << "    ";
    for (RangeTree2D::iterator it = rangeTree2D.begin(); it != rangeTree2D.end(); it++) {
        std::cout << *it  << " ";
    }
    std::cout << std::endl;


    std::cout << std::endl;



    /* ----- OTHER FUNCTIONS ----- */

    //Get min and max (through iterators)
    RangeTree2D::iterator minIterator = rangeTree2D.getMin();
    if (minIterator != rangeTree2D.end()) {
        Point2Dd& minPointString = *minIterator;
        std::cout << "Minimum node is: " << minPointString << std::endl;
    }
    RangeTree2D::iterator maxIterator = rangeTree2D.getMax();
    if (maxIterator != rangeTree2D.end()) {
        std::cout << "Maximum node is: " << *maxIterator << std::endl;
    }


    //Size
    std::cout << "The range tree contains " << rangeTree2D.size() << " elements" << std::endl;

    //Clear
    std::cout << "Clear range tree..." << std::endl;
    rangeTree2D.clear();

    std::cout << std::endl;






    //Example for range trees with pointers to save memory: indeed in every dimension range trees
    //both keys and values are copied and saved. If you have big size objects (for both keys and
    //value types), please use this solution to save memory. The implementation choice has been
    //made to allow the users to easily implement (if needed) range queries on subsets of the
    //dimensions of the range tree.
    std::cout << "Creating range trees with pointers to save space!..." << std::endl;

    //Defining an alias for range tree
    //The second argument type is the value associated to the node with
    //a key (first argument type) and it must have the copy constructor
    typedef cg3::RangeTree<Point2Dd*, std::string*> RangeTreeWithPointers;

    //Adding comparators for pointers
    std::vector<RangeTreeWithPointers::DefaultComparator> customPointerComparators;
    customPointerComparators.push_back(&point2DPointerDimensionComparatorX);
    customPointerComparators.push_back(&point2DPointerDimensionComparatorY);

    //Creating vector of pairs
    std::cout << "Creating range tree initialized with: [1,10], [5,13]" << std::endl;

    Point2Dd* p1 = new Point2Dd(1,10);
    Point2Dd* p2 = new Point2Dd(5,13);
    std::string* s1 = new std::string("[1,10]");
    std::string* s2 = new std::string("[5,13]");

    std::vector<std::pair<Point2Dd*, std::string*>> constructionVec;
    constructionVec.push_back(std::make_pair(p1, s1));
    constructionVec.push_back(std::make_pair(p2, s2));

    //Creating range tree (for custom objects)
    //Construction vector is optional
    RangeTreeWithPointers rangeTreeWithPointers(
        2, constructionVec, customPointerComparators
    );

    //Insert objects: [1,10], [5,13], [12,12], [4,11]
    std::cout << "Inserting objects: [12,12], [4,11]" << std::endl;

    Point2Dd* p3 = new Point2Dd(12,12);
    Point2Dd* p4 = new Point2Dd(4,11);
    std::string* s3 = new std::string("[12,12]");
    std::string* s4 = new std::string("[4,11]");
    rangeTreeWithPointers.insert(p3, s3);
    rangeTreeWithPointers.insert(p4, s4);

    //Find object [1,10]
    Point2Dd* queryObject = new Point2Dd(1,10);
    RangeTreeWithPointers::iterator queryPointerIterator = rangeTreeWithPointers.find(queryObject);
    if (queryPointerIterator != rangeTreeWithPointers.end())
        std::cout << "Object [1,10] is in the range tree!" << std::endl;
    else
        std::cout << "Object [1,10] is NOT in the range tree!" << std::endl;
    delete queryObject;

    //Erase
    Point2Dd* eraseObject = new Point2Dd(1,10);
    std::cout << "Erasing object [1,10]..." << std::endl;
    rangeTreeWithPointers.erase(eraseObject);
    delete eraseObject;

    //Iteration
    std::cout << "The range tree contains:" << std::endl << "    ";
    for (std::string* objectString : rangeTreeWithPointers)
        std::cout << *objectString << " ";
    std::cout << std::endl;

    //Range query for the interval [3 - 15, 12 - 13]
    std::cout << "Range query for the interval [3 - 15, 12 - 13]:" << std::endl << "    ";
    std::vector<RangeTreeWithPointers::iterator> rangeQueryPointerResults;
    Point2Dd* startPointer = new Point2Dd(3,12);
    Point2Dd* endPointer = new Point2Dd(15,13);

    rangeTreeWithPointers.rangeQuery(
                startPointer,
                endPointer,
                std::back_inserter(rangeQueryPointerResults));

    for (RangeTreeWithPointers::iterator& it : rangeQueryPointerResults) {
        std::cout << *(*it) << " ";
    }
    std::cout << std::endl;


    //Deleting data
    delete startPointer;
    delete endPointer;

    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete s1;
    delete s2;
    delete s3;
    delete s4;

}



/* ---- COMPARATORS FOR POINTERS IMPLEMENTATION ----- */

bool point2DPointerDimensionComparatorX(
        Point2Dd* const& o1,
        Point2Dd* const& o2)
{
    if (o1->x() < o2->x()) {
        return true;
    }
    if (o2->x() < o1->x()) {
        return false;
    }
    return o1->y() < o2->y();
}

bool point2DPointerDimensionComparatorY(
        Point2Dd* const& o1,
        Point2Dd* const& o2)
{
    if (o1->y() < o2->y()) {
        return true;
    }
    if (o2->y() < o1->y()) {
        return false;
    }
    return o1->x() < o2->x();
}

