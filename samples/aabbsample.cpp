/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "aabbsample.h"

#include <iostream>

#include <set>

#include <cg3/data_structures/trees/aabbtree.h>

#include <cg3/geometry/2d/point2d.h>
#include <cg3/geometry/segment.h>

typedef cg3::AABBValueType AABBValueType;
typedef cg3::Point2D<int> Point2D;
typedef cg3::Segment<Point2D> Segment2D;


double aabbValueExtractor(
        const Segment2D& segment,
        const AABBValueType& valueType,
        const int& dim);
bool segment2DIntersectionChecker(const Segment2D& segment1, const Segment2D& segment2);
bool segment2DCustomComparator(const Segment2D& o1, const Segment2D& o2);


void AABBSample::execute()
{

    /* ----- BASIC USAGE ----- */

    //Defining an alias for a 2-dimensional AABB tree
    //The second argument type must have the copy constructor
    typedef cg3::AABBTree<2, Segment2D, std::string> AABBTree;


    //Creating vector for construction with object ([2,4], [3,1]) | ([5,9], [8,1])
    std::cout << "Construction objects: ([2,4], [3,1]) | ([5,9], [8,1])" << std::endl;
    std::vector<std::pair<Segment2D, std::string>> segmentVec;
    segmentVec.push_back(std::make_pair(Segment2D(Point2D(2,4),Point2D(3,1)), "([2,4], [3,1])"));
    segmentVec.push_back(std::make_pair(Segment2D(Point2D(5,9),Point2D(8,1)), "([5,9], [8,1])"));


    //Creating AABBtree (for Segment2D)
    std::cout << "Creating AABB trees with comparators..." << std::endl;
    AABBTree aabbTree(
                segmentVec,
                &aabbValueExtractor, //AABB value extractor
                &segment2DCustomComparator); //Comparator (if omitted, < is the default)


    //Insert objects: ([-1,-5], [3,2]) | ([1,2], [4,3]) | ([4,3], [4,8])
    std::cout << "Inserting objects: ([-1,-5], [3,2]) | ([1,2], [4,3]) | ([4,3], [4,8])" << std::endl;
    aabbTree.insert(Segment2D(Point2D(-1,-5),Point2D(3,2)), "([-1,-5], [3,2])");
    aabbTree.insert(Segment2D(Point2D(1,2),Point2D(4,3)), "([1,2], [4,3])");
    aabbTree.insert(Segment2D(Point2D(4,3),Point2D(4,8)), "([4,3], [4,8])");

    //Iteration
    std::cout << "The AABB tree contains:" << std::endl << "    ";
    for (std::string& objectString : aabbTree)
        std::cout << objectString << " | ";
    std::cout << std::endl;


    //Find object ([2,4], [3,1])
    AABBTree::iterator queryIterator = aabbTree.find(Segment2D(Point2D(2,4),Point2D(3,1)));
    if (queryIterator != aabbTree.end())
        std::cout << "Object ([2,4], [3,1]) is in the AABB tree!" << std::endl;
    else
        std::cout << "Object ([2,4], [3,1]) is NOT in the AABB tree!" << std::endl;

    //Erase
    std::cout << "Erasing object ([2,4], [3,1])..." << std::endl;
    aabbTree.erase(Segment2D(Point2D(2,4),Point2D(3,1)));

    //Iteration with explicit iterators
    std::cout << "The AABB tree contains:" << std::endl << "    ";
    for (AABBTree::iterator it = aabbTree.begin(); it != aabbTree.end(); it++) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;

    std::cout << std::endl;



     /* ----- QUERY FUNCTIONS ----- */


    //Overlap query (only overlaps of AABBs) for the segment ([0,3], [8,10])
    std::cout << "AABB overlaps: segment ([0,3], [8,10]) -> ";
    if (aabbTree.aabbOverlapCheck(Segment2D(Point2D(0,3),Point2D(8,10)))) {
        std::cout << "Found at least an overlap";
    }
    else {
        std::cout << "Found NO overlaps";
    }
    std::cout << std::endl << " ->    ";

    std::vector<AABBTree::iterator> aabbQueryResults;
    aabbTree.aabbOverlapQuery(
                Segment2D(Point2D(0,3),Point2D(8,10)),
                std::back_inserter(aabbQueryResults));

    for (AABBTree::iterator& it : aabbQueryResults) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;


    //Overlap query (with custom intersection checker) for the segment ([0,3], [8,10])
    std::cout << "Intersections with custom intersection checker: segment ([0,3], [8,10]) -> ";
    if (aabbTree.aabbOverlapCheck(Segment2D(Point2D(0,3),Point2D(8,10))), &segment2DIntersectionChecker) {
        std::cout << "Found at least an overlap";
    }
    else {
        std::cout << "Found NO overlaps";
    }
    std::cout << std::endl << " ->    ";

    std::vector<AABBTree::iterator> intersectionQueryResults;
    aabbTree.aabbOverlapQuery(
                Segment2D(Point2D(0,3),Point2D(8,10)),
                std::back_inserter(intersectionQueryResults),
                &segment2DIntersectionChecker);

    for (AABBTree::iterator& it : intersectionQueryResults) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;



    //Overlap query (with custom intersection checker) for the segment ([20,30], [80,10])
    std::cout << "Intersections with custom intersection checker: segment ([20,30], [80,10]) -> ";
    if (aabbTree.aabbOverlapCheck(Segment2D(Point2D(20,30),Point2D(80,10)), &segment2DIntersectionChecker)) {
        std::cout << "Found at least an overlap";
    }
    else {
        std::cout << "Found NO overlaps";
    }
    std::cout << std::endl << " ->    ";

    std::vector<AABBTree::iterator> intersectionQueryResults2;
    aabbTree.aabbOverlapQuery(
                Segment2D(Point2D(20,30),Point2D(80,10)),
                std::back_inserter(intersectionQueryResults2),
                &segment2DIntersectionChecker);

    for (AABBTree::iterator& it : intersectionQueryResults2) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;

    std::cout << std::endl;



    /* ----- OTHER FUNCTIONS ----- */

    //Get min and max (through iterators)
    AABBTree::iterator minIterator = aabbTree.getMin();
    if (minIterator != aabbTree.end()) {
        std::string& minObjectString = *minIterator;
        std::cout << "Minimum node is: " << minObjectString << std::endl;
    }
    AABBTree::iterator maxIterator = aabbTree.getMax();
    if (maxIterator != aabbTree.end()) {
        std::cout << "Maximum node is: " << *maxIterator << std::endl;
    }

    //Size
    std::cout << "The AABB tree contains " << aabbTree.size() << " elements" << std::endl;

    //Iteration with explicit iterators
    std::cout << "The AABB tree contains (reverse):" << std::endl << "    ";
    for (AABBTree::reverse_iterator it = aabbTree.rbegin(); it != aabbTree.rend(); it++) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;

    //Clear
    std::cout << "Clear AABB tree..." << std::endl;
    aabbTree.clear();


    std::cout << std::endl;
}



/*
 * Function to extract the values of the AABB from a segment
 */
double aabbValueExtractor(
        const Segment2D& segment,
        const AABBValueType& valueType,
        const int& dim)
{
    if (valueType == AABBValueType::MIN) {
        switch (dim) {
        case 1:
            return (double) std::min(segment.p1().x(), segment.p2().x());
        case 2:
            return (double) std::min(segment.p1().y(), segment.p2().y());
        }
    }
    if (valueType == AABBValueType::MAX) {
        switch (dim) {
        case 1:
            return (double) std::max(segment.p1().x(), segment.p2().x());
        case 2:
            return (double) std::max(segment.p1().y(), segment.p2().y());
        }
    }
    assert(false);
    return false;
}


/*
 * Returns true if the two segment have an intersection
 * or they collide
 */
bool segment2DIntersectionChecker(const Segment2D& segment1, const Segment2D& segment2) {
    double s1MinX = std::min(segment1.p1().x(), segment1.p2().x());
    double s2MinX = std::min(segment2.p1().x(), segment2.p2().x());
    double s1MaxX = std::max(segment1.p1().x(), segment1.p2().x());
    double s2MaxX = std::max(segment2.p1().x(), segment2.p2().x());

    double s1MinY = std::min(segment1.p1().y(), segment1.p2().y());
    double s2MinY = std::min(segment2.p1().y(), segment2.p2().y());
    double s1MaxY = std::max(segment1.p1().y(), segment1.p2().y());
    double s2MaxY = std::max(segment2.p1().y(), segment2.p2().y());

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


/*
 * Default comparator for a 2D segment. It is needed whether
 * no operator < has been defined on segment2D class. It is
 * used to determine whether an object is a duplicated,
 * if the equality test with the dimension comparator has
 * returned true
 */
bool segment2DCustomComparator(const Segment2D& o1, const Segment2D& o2) {
    if (o1.p1() < o2.p1())
        return true;
    if (o2.p2() < o1.p1())
        return false;
    return o1.p2() < o2.p2();
}
