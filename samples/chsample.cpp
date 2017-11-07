/**
    @author Stefano Nuvoli
*/
#include "chsample.h"

#include <iostream>

#include <set>

#include "cg3/algorithms/convexhull2d.h"

#include "cg3/geometry/2d/point2d.h"

#define PLANE_WIDTH 10
#define PLANE_HEIGHT 10

typedef cg3::Point2D<int> Point2D;

void ConvexHullSamples::sampleConvexHull()
{

    //Insert input points
    std::vector<Point2D> points;
    points.push_back(Point2D(3,5));
    points.push_back(Point2D(0,3));
    points.push_back(Point2D(4,5));
    points.push_back(Point2D(2,8));
    points.push_back(Point2D(6,3));
    points.push_back(Point2D(4,1));
    points.push_back(Point2D(0,1));
    points.push_back(Point2D(2,3));
    points.push_back(Point2D(2,1));
    points.push_back(Point2D(8,8));

    //Print input points
    std::cout << "Input points:" << std::endl;
    for (std::vector<Point2D>::iterator it = points.begin(); it != points.end(); it++) {
        Point2D point = *it;
        std::cout << "(" << point.x() << "," << point.y() << ")" << " - ";
    }
    std::cout << std::endl;


    //Representation of the cartesian plane
    char plane[PLANE_WIDTH][PLANE_HEIGHT];



    /**** GRAHAM SCAN *****/

    std::cout << std::endl << std::endl;

    std::cout << "***** GRAHAM SCAN *****" << std::endl << std::endl;

    //Clear/initialize the plane
    for (int x = 0; x < PLANE_WIDTH; x++) {
        for (int y = 0; y < PLANE_HEIGHT; y++) {
            plane[x][y] = ' ';
        }
    }

    //Set all initial points to "o"
    for (std::vector<Point2D>::iterator it = points.begin(); it != points.end(); it++) {
        plane[it->x()][it->y()] = 'o';
    }

    //Computing convex hull
    std::list<Point2D> convexHull;
    //Template <double> would be not needed
    cg3::getConvexHull2D<int>(points, convexHull);


    //Set all convex hull points to "x"
    for (std::list<Point2D>::iterator it = convexHull.begin(); it != convexHull.end(); it++) {
        plane[it->x()][it->y()] = 'x';
    }

    //Draw the plane
    for ( int y = PLANE_HEIGHT-1; y >= 0 ; --y ) {
        std::cout << y << "  " ;
        for ( int x = 0; x < PLANE_WIDTH; ++x ) {
            std::cout << plane[x][y] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "   ";
    for ( int x = 0; x < PLANE_WIDTH; ++x ) {
        std::cout << x << " ";
    }
    std::cout << std::endl << std::endl;

    //Printing solution
    std::cout << "Solution:" << std::endl;
    for (std::list<Point2D>::iterator it = convexHull.begin(); it != convexHull.end(); it++) {
        Point2D point = *it;
        std::cout << "(" << point.x() << "," << point.y() << ")" << " - ";
    }
    std::cout << std::endl;



    /**** ITERATIVE CONVEX HULL *****/

    std::cout << std::endl << std::endl;

    std::cout << "***** ITERATIVE CONVEX HULL *****" << std::endl << std::endl;

    //Clear/initialize the plane
    for (int x = 0; x < PLANE_WIDTH; x++) {
        for (int y = 0; y < PLANE_HEIGHT; y++) {
            plane[x][y] = ' ';
        }
    }

    //Set all initial points to "o"
    for (std::vector<Point2D>::iterator it = points.begin(); it != points.end(); it++) {
        plane[it->x()][it->y()] = 'o';
    }
}

