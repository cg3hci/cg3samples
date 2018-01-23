/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "chsample.h"

#include <iostream>

#include <vector>

#include <cg3/algorithms/2d/convexhull2d.h>
#include <cg3/algorithms/2d/convexhull2d_iterative.h>

#include <cg3/geometry/2d/point2d.h>

#define PLANE_WIDTH 10
#define PLANE_HEIGHT 10

typedef cg3::Point2D<int> Point2D;

void CHSamples::execute()
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



    /* ----- GRAHAM SCAN ----- */

    std::cout << std::endl << std::endl;

    std::cout << " >> GRAHAM SCAN" << std::endl << std::endl;

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

    //Output container
    std::list<Point2D> outputConvexHull;

    //Computing convex hull
    //Template <double> would be not needed
    cg3::getConvexHull2D<int>(points.begin(), points.end(), std::back_inserter(outputConvexHull));

    //Alternative with containers
//    cg3::getConvexHull2D<int>(points, outputConvexHull);


    //Set all convex hull points to "x"
    for (std::list<Point2D>::iterator it = outputConvexHull.begin(); it != outputConvexHull.end(); it++) {
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
    for (std::list<Point2D>::iterator it = outputConvexHull.begin(); it != outputConvexHull.end(); it++) {
        Point2D point = *it;
        std::cout << "(" << point.x() << "," << point.y() << ")" << " - ";
    }
    std::cout << std::endl;



    /* ----- ITERATIVE CONVEX HULL ----- */

    std::cout << std::endl << std::endl;

    std::cout << " >> ITERATIVE CONVEX HULL" << std::endl << std::endl;

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



    //Add points in different ways
    cg3::IterativeConvexHull2D<int> iterativeConvexHull(points.begin(), points.end()-4);

    iterativeConvexHull.addPoints(points.end()-4, points.end()-3);

    Point2D p2 = *(points.end()-2);
    iterativeConvexHull.addPoint(p2);

    Point2D p1 = *(points.end()-1);
    cg3::addPointToConvexHull(p1, iterativeConvexHull);

    //Alternative with container
//    cg3::IterativeConvexHull2D<int> iterativeConvexHull(points);

    //Output convex hull
    std::list<Point2D> outputIterativeConvexHull;
    iterativeConvexHull.getConvexHull(std::back_inserter(outputIterativeConvexHull));

    //Set all convex hull points to "x"
    for (std::list<Point2D>::iterator it = outputIterativeConvexHull.begin(); it != outputIterativeConvexHull.end(); it++) {
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
    for (std::list<Point2D>::iterator it = outputIterativeConvexHull.begin(); it != outputIterativeConvexHull.end(); it++) {
        Point2D point = *it;
        std::cout << "(" << point.x() << "," << point.y() << ")" << " - ";
    }
    std::cout << std::endl;

    iterativeConvexHull.clear();
}

