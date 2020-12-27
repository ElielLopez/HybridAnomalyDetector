//
// Created by eliel on 27/12/2020.
//

#ifndef EX4_MINCIRCLE_H
#define EX4_MINCIRCLE_H

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//    float x,y;
//    Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

/*
 * I implemented a naive algorithm that runs at O(n^2) in theory.
 * but in fact runs even less- around 3000ms due to conditions
 * that filters irrelevant points.
 * */

class minCircle{
    const float INF = 1e18;

public:

// return the middle point by equation: x = (x1-x2) / 2, y = (y1-y2) / 2.
    Point calcMiddle(Point a, Point b);

// return the distance between two point by the equation:
// D = sqrt((x1-x2)^2 + (y1-y2)^2)
    float calcDistance(Point a, Point b);

// given two points, calculate the center point and the radius and creates a small circle
// that the two points are on the circle
    Circle createTwoPointCircle(Point a, Point b);

// checks if a given Point is outside the boundaries of the circle.
    bool isInsideCircle(Circle c, Point a);

// checks if all points are inside the circle. if even a single point is outside- return false.
    bool isValidMEC(Circle c, Point** points, size_t size);

// return a minimal circle, enclosing all points in 2D plane.
    Circle findMinCircle(Point** points,size_t size);
};

#endif //EX4_MINCIRCLE_H
