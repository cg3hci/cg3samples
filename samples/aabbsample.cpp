/**
    @author Stefano Nuvoli
*/
#include "aabbsample.h"

#include <iostream>

#include <set>

#include "cg3/data_structures/trees/aabbtree.h"

#include "cg3/geometry/2d/point2d.h"
#include "cg3/geometry/segment.h"

typedef cg3::AABBValueType AABBValueType;
typedef cg3::Point2D<int> Point2D;
typedef cg3::Segment<Point2D> Segment2D;



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
}


/*
 * Returns true if the two segment have an intersection
 * or they collide
 */
bool segment2DIntersectionChecker(const Segment2D& segment1, const Segment2D& segment2) {
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


/*
 * Default comparator for a 2D segment. It is needed whether
 * no operator < has been defined on segment2D class. It is
 * used to determine whether an object is a duplicated,
 * if the equality test with the dimension comparator has
 * returned true
 */
bool segment2DCustomComparator(const Segment2D& o1, const Segment2D& o2) {
    if (o1.getP1() < o2.getP1())
        return true;
    if (o2.getP2() < o1.getP1())
        return false;
    return o1.getP2() < o2.getP2();
}


void AABBTreeSamples::sampleAABBTree()
{
    //Defining an alias for a 2-dimensional AABB tree
    //The second argument type must have the copy constructor
    typedef cg3::AABBTree<2, Segment2D, std::string> AABBTree;


    //Creating AABBtree (for Segment2D)
    std::cout << "Creating AABB trees with comparators..." << std::endl;
    AABBTree aabbTree(&aabbValueExtractor, //AABB value extractor
                      &segment2DCustomComparator); //Default comparator (< is the default)


    //Insert objects: ([2,4], [3,1]) | ([5,9], [8,1]) | ([-1,-5], [3,2]) | ([1,2], [4,3]) | ([4,3], [4,8])
    std::cout << "Inserting objects: ([2,4], [3,1]) | ([5,9], [8,1]) | ([-1,-5], [3,2]) | ([1,2], [4,3]) | ([4,3], [4,8])" << std::endl;
    aabbTree.insert(Segment2D(Point2D(2,4),Point2D(3,1)), "([2,4], [3,1])");
    aabbTree.insert(Segment2D(Point2D(5,9),Point2D(8,1)), "([5,9], [8,1])");
    aabbTree.insert(Segment2D(Point2D(-1,-5),Point2D(3,2)), "([-1,-5], [3,2])");
    aabbTree.insert(Segment2D(Point2D(1,2),Point2D(4,3)), "([1,2], [4,3])");
    aabbTree.insert(Segment2D(Point2D(4,3),Point2D(4,8)), "([4,3], [4,8])");

    //Iteration
    std::cout << "The AABB tree contains:" << std::endl << "    ";
    for (std::string& objectString : aabbTree)
        std::cout << objectString << " | ";
    std::cout << std::endl;

    //Get min and max (through iterators)
    AABBTree::Iterator minIterator = aabbTree.getMin();
    std::string& minObjectString = *minIterator;
    std::cout << "Minimum node is: " << minObjectString << std::endl;

    std::cout << "Maximum node is: " << *(aabbTree.getMax()) << std::endl;

    //Find object ([2,4], [3,1])
    AABBTree::Iterator queryIterator = aabbTree.find(Segment2D(Point2D(2,4),Point2D(3,1)));
    if (queryIterator != aabbTree.end())
        std::cout << "Object ([2,4], [3,1]) is in the AABB tree!" << std::endl;
    else
        std::cout << "Object ([2,4], [3,1]) is NOT in the AABB tree!" << std::endl;

    //Erase
    std::cout << "Erasing object ([2,4], [3,1])..." << std::endl;
    aabbTree.erase(Segment2D(Point2D(2,4),Point2D(3,1)));

    //Iteration with explicit iterators
    std::cout << "The AABB tree contains:" << std::endl << "    ";
    for (AABBTree::Iterator it = aabbTree.begin(); it != aabbTree.end(); it++) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;



    //Overlap query (only overlaps of AABBs) for the segment ([0,3], [8,10])
    std::cout << "AABB overlaps: segment ([0,3], [8,10]) -> ";
    if (aabbTree.aabbOverlapCheck(Segment2D(Point2D(0,3),Point2D(8,10)))) {
        std::cout << "Found at least an overlap";
    }
    else {
        std::cout << "Found NO overlaps";
    }
    std::cout << std::endl;

    std::cout << "Query:" << std::endl << "    ";
    std::vector<AABBTree::Iterator> aabbQueryResults;
    aabbTree.aabbOverlapQuery(Segment2D(Point2D(0,3),Point2D(8,10)), aabbQueryResults);
    for (AABBTree::Iterator& it : aabbQueryResults) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;


    //Overlap query (with custom intersection checker) for the segment ([0,3], [8,10])
    std::cout << "Intersections: segment ([0,3], [8,10]) -> ";
    if (aabbTree.aabbOverlapCheck(Segment2D(Point2D(0,3),Point2D(8,10))), &segment2DIntersectionChecker) {
        std::cout << "Found at least an overlap";
    }
    else {
        std::cout << "Found NO overlaps";
    }
    std::cout << std::endl;

    std::cout << "Query:" << std::endl << "    ";
    std::vector<AABBTree::Iterator> intersectionQueryResults;
    aabbTree.aabbOverlapQuery(Segment2D(Point2D(0,3),Point2D(8,10)), intersectionQueryResults, &segment2DIntersectionChecker);
    for (AABBTree::Iterator& it : intersectionQueryResults) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;



    //Overlap query (with custom intersection checker) for the segment ([20,30], [80,10])
    std::cout << "Intersections: segment ([20,30], [80,10]) -> ";
    if (aabbTree.aabbOverlapCheck(Segment2D(Point2D(20,30),Point2D(80,10)), &segment2DIntersectionChecker)) {
        std::cout << "Found at least an overlap";
    }
    else {
        std::cout << "Found NO overlaps";
    }
    std::cout << std::endl;

    std::cout << "Query:" << std::endl << "    ";
    std::vector<AABBTree::Iterator> intersectionQueryResults2;
    aabbTree.aabbOverlapQuery(Segment2D(Point2D(20,30),Point2D(80,10)), intersectionQueryResults2, &segment2DIntersectionChecker);
    for (AABBTree::Iterator& it : intersectionQueryResults2) {
        std::cout << *it << " | ";
    }
    std::cout << std::endl;

    //Size
    std::cout << "The AABB tree contains " << aabbTree.size() << " elements" << std::endl;

    //Clear
    std::cout << "Clear AABB tree..." << std::endl;
    aabbTree.clear();



    std::cout << std::endl;
}
