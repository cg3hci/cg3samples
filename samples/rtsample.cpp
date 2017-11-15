/**
    @author Stefano Nuvoli
*/
#include "rtsample.h"

#include <iostream>

#include <set>

#include "cg3/data_structures/trees/rangetree.h"

#include "cg3/geometry/2d/point2d.h"

typedef cg3::Point2D<double> Point2D;


bool point2DPointerDimensionComparatorX(
        Point2D* const& o1,
        Point2D* const& o2);

bool point2DPointerDimensionComparatorY(
        Point2D* const& o1,
        Point2D* const& o2);


bool point2DDimensionComparatorX(
        const Point2D& o1,
        const Point2D& o2);

bool point2DDimensionComparatorY(
        const Point2D& o1,
        const Point2D& o2);


bool point2DComparator(
        const Point2D& o1,
        const Point2D& o2);

bool point2DCustomComparator(const Point2D& o1, const Point2D& o2);

void RTSample::execute()
{
    //Defining an alias for range tree
    //The second argument type must have the copy constructor
    typedef cg3::RangeTree<Point2D, std::string> RangeTree;



    /* ----- BASIC USAGE ----- */

    //Creating vector of comparators
    std::vector<RangeTree::LessComparator> customComparators;
    customComparators.push_back(&point2DDimensionComparatorX);
    customComparators.push_back(&point2DDimensionComparatorY);

    //Creating 2-dimensional-range-tree (for Point2D)
    std::cout << "Creating range trees with comparators..." << std::endl;
    RangeTree rangeTree(2, //Dimension of the range tree
                        customComparators); //Vector of comparators

    //Insert objects: [12,15.9], [2,10.1], [45,65.0], [100,52.25], [45,20], [3,25.0]
    std::cout << "Inserting objects: [12,15.9], [2,10.1], [45,65.0], [100,52.25], [45,20], [3,25.0]" << std::endl;
    rangeTree.insert(Point2D(12,15.9), "[12,15.9]");
    rangeTree.insert(Point2D(2,10.1), "[2,10.1]");
    rangeTree.insert(Point2D(45,65.0), "[45,65.0]");
    rangeTree.insert(Point2D(100,52.25), "[100,52.25]");
    rangeTree.insert(Point2D(45,20), "[45,20]");
    rangeTree.insert(Point2D(3,25.0), "[3,25.0]");

    //Iteration
    std::cout << "The range tree contains:" << std::endl << "    ";
    for (std::string& objectString : rangeTree)
        std::cout << objectString << " ";
    std::cout << std::endl;

    //Find object [2,10.1]
    RangeTree::iterator queryIterator = rangeTree.find(Point2D(2,10.1));
    if (queryIterator != rangeTree.end())
        std::cout << "Object [2,10.1] is in the range tree!" << std::endl;
    else
        std::cout << "Object [2,10.1] is NOT in the range tree!" << std::endl;

    //Erase
    std::cout << "Erasing object [2,10.1]..." << std::endl;
    rangeTree.erase(Point2D(2,10.1));

    //Range query for the interval [3 - 99, 15.99 - 65.0]
    std::cout << "Range query for the interval [3 - 99, 15.99 - 65.0]:" << std::endl << "    ";
    std::vector<RangeTree::iterator> rangeQueryResults;
    rangeTree.rangeQuery(Point2D(3,15.99), Point2D(99,65.0), rangeQueryResults);
    for (RangeTree::iterator it : rangeQueryResults) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    //Iteration with explicit iterators
    std::cout << "The range tree contains:" << std::endl << "    ";
    for (RangeTree::iterator it = rangeTree.begin(); it != rangeTree.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;


    std::cout << std::endl;


    /* ----- OTHER FUNCTION ----- */

    //Get min and max (through iterators)
    RangeTree::iterator minIterator = rangeTree.getMin();
    if (minIterator != rangeTree.end()) {
        std::string& minPointString = *minIterator;
        std::cout << "Minimum node is: " << minPointString << std::endl;
    }
    RangeTree::iterator maxIterator = rangeTree.getMax();
    if (maxIterator != rangeTree.end()) {
        std::cout << "Maximum node is: " << *maxIterator << std::endl;
    }


    //Size
    std::cout << "The range tree contains " << rangeTree.size() << " elements" << std::endl;

    //Clear
    std::cout << "Clear range tree..." << std::endl;
    rangeTree.clear();

    std::cout << std::endl;






    //Example for range trees with pointers to save memory: indeed in every dimension range trees
    //both keys and values are copied and saved. If you have big size objects (for both keys and
    //value types), please use this solution to save memory. The implementation choice has been
    //made to allow the users to easily implement (if needed) range queries on subsets of the
    //dimensions of the range tree.
    std::cout << "Creating range trees with pointers to save space!..." << std::endl;


    //Defining an alias for range tree
    //The second argument type must have the copy constructor
    typedef cg3::RangeTree<Point2D*, std::string*> RangeTreeWithPointers;

    //Adding comparators for pointers
    std::vector<RangeTreeWithPointers::LessComparator> customPointerComparators;
    customPointerComparators.push_back(&point2DPointerDimensionComparatorX);
    customPointerComparators.push_back(&point2DPointerDimensionComparatorY);

    //Creating range tree
    RangeTreeWithPointers rangeTreeWithPointers(
        2, customPointerComparators
    );

    //Insert objects: [1,10], [5,13], [12,12], [4,11]
    std::cout << "Inserting objects: [1,10], [5,13], [12,12], [4,11]" << std::endl;
    Point2D* p1 = new Point2D(1,10);
    Point2D* p2 = new Point2D(5,13);
    Point2D* p3 = new Point2D(12,12);
    Point2D* p4 = new Point2D(4,11);
    std::string* s1 = new std::string("[1,10]");
    std::string* s2 = new std::string("[5,13]");
    std::string* s3 = new std::string("[12,12]");
    std::string* s4 = new std::string("[4,11]");
    rangeTreeWithPointers.insert(p1, s1);
    rangeTreeWithPointers.insert(p2, s2);
    rangeTreeWithPointers.insert(p3, s3);
    rangeTreeWithPointers.insert(p4, s4);

    //Find object [1,10]
    Point2D* queryObject = new Point2D(1,10);
    RangeTreeWithPointers::iterator queryPointerIterator = rangeTreeWithPointers.find(queryObject);
    if (queryPointerIterator != rangeTreeWithPointers.end())
        std::cout << "Object [1,10] is in the range tree!" << std::endl;
    else
        std::cout << "Object [1,10] is NOT in the range tree!" << std::endl;
    delete queryObject;

    //Erase
    Point2D* eraseObject = new Point2D(1,10);
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
    Point2D* startPointer = new Point2D(3,12);
    Point2D* endPointer = new Point2D(15,13);
    rangeTreeWithPointers.rangeQuery(startPointer, endPointer, rangeQueryPointerResults);
    for (RangeTreeWithPointers::iterator& it : rangeQueryPointerResults) {
        std::cout << *(*it) << " ";
    }
    std::cout << std::endl;
    delete startPointer;
    delete endPointer;

    //Deleting data
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete s1;
    delete s2;
    delete s3;
    delete s4;

}


bool point2DPointerDimensionComparatorX(
        Point2D* const& o1,
        Point2D* const& o2)
{
    return point2DDimensionComparatorX(*o1,*o2);
}
bool point2DPointerDimensionComparatorY(
        Point2D* const& o1,
        Point2D* const& o2)
{
    return point2DDimensionComparatorY(*o1,*o2);
}



bool point2DDimensionComparatorX(
        const Point2D& o1,
        const Point2D& o2)
{
    if (o1.x() < o2.x())
        return true;
    if (o2.x() < o1.x())
        return false;
    return point2DComparator(o1,o2);
}
bool point2DDimensionComparatorY(
        const Point2D& o1,
        const Point2D& o2)
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
