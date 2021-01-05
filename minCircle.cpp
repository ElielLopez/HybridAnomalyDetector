//
// Created by eliel on 27/12/2020.
//
#include "minCircle.h"
#include <math.h>

// return the middle point by equation: x = (x1-x2) / 2, y = (y1-y2) / 2.
Point minCircle::calcMiddle(Point a, Point b) {

    float ax, ay;
    float bx, by;
    float x, y;

    ax = a.x;
    ay = a.y;
    bx = b.x;
    by = b.y;

    x = (ax + bx) / 2;
    y = (ay + by) / 2;

    Point middle(x, y);

    return middle;
}

// return the distance between two point by the equation:
// D = sqrt((x1-x2)^2 + (y1-y2)^2)
float minCircle::calcDistance(Point a, Point b) {

    float distance;
    float ax, ay;
    float bx, by;
    float x, y;

    ax = a.x;
    ay = a.y;
    bx = b.x;
    by = b.y;

    x = ax - bx;
    y = ay - by;

    x = pow(x, 2);
    y = pow(y, 2);

    distance = x + y;
    distance = sqrtf(distance);

    return distance;
}

// given two points, calculate the center point and the radius and creates a small circle
// that the two points are on the circle
Circle minCircle::createTwoPointCircle(Point a, Point b) {

    float radius;

    auto center = calcMiddle(a, b);
    radius = calcDistance(a, b);
    radius /= 2;

    Circle c(center, radius);

    return c;
}

// checks if a given Point is outside the boundaries of the circle.
bool minCircle::isInsideCircle(Circle c, Point a) {
    return calcDistance(c.center, a) <= c.radius;
}

// checks if all points are inside the circle. if even a single point is outside- return false.
bool minCircle::isValidMEC(Circle c, Point** points, size_t size) {
    for(int i = 0; i < size; i++) {
        if(!isInsideCircle(c, Point(points[i]->x, points[i]->y))) return false;
    }
    return true;
}

// return a minimal circle, enclosing all points in 2D plane.
Circle minCircle::findMinCircle(Point** points,size_t size) {

//    for(int i=0;i<size;i++)
//        cout<<points[i]->x<<" --- "<<points[i]->y<<endl;

    // first checks for trivial size of 0,1,2,3.
    if(size == 0) return {{0, 0}, 0};
    if(size == 1) return {Point(points[0]->x, points[0]->y), 0};
    if(size == 2) return createTwoPointCircle(Point(points[0]->x, points[0]->y),
                                              Point(points[1]->x, points[1]->y));

    Point p1(points[0]->x, points[0]->y);
    Point p2(points[1]->x, points[1]->y);

    // default circle.
//    float radius;
//    radius = this->calcDistance(p1, p2);
//    radius /= 2;

    //Circle mec = {p1, INF};
    Circle mec = createTwoPointCircle(p1, p2);

    // for every 2 points, create a circle and then check its validity and size.
    // if smaller then the current circle- update it.
    for(int i = 0; i < size; i++) {

        // this condition helps reduce the time significantly
        // because its ignoring points that are inside the circle.
        for(int j = i + 1; j < size ; j++) {

            if(!isInsideCircle(mec, Point(points[j]->x, points[j]->y))) {
                Circle tmp = createTwoPointCircle(Point(points[i]->x, points[i]->y),
                                                  Point(points[j]->x, points[j]->y));
                if(tmp.radius < mec.radius && isValidMEC(tmp, points, size)) {
                    mec = tmp;
                }
            }
        }

    }

    return mec;
}
