#ifndef __POINT_H
#define __POINT_H

#include "vector.h"

#include <math.h>

class point {
public:
    point() {}
    point(double x, double y) : x(x), y(y) {}
    virtual ~point() {}

    point(const point& other) {
        *this = other;
    }

    point& operator = (const point& other) {
        x = other.x;
        y = other.y;
        return *this;
    }

public:
    double x;
    double y;

public:
    double distance(point& pt) {
        double dx = pt.x - x;
        double dy = pt.y - y;
        return sqrt(dx*dx+dy*dy);
    }

public:
    point operator + (const vector2d& vec) {
        return point(x+vec.x, y+vec.y);
    }
    point& operator += (const vector2d& vec) {
        return *this = *this + vec;
    }
    point operator - (const vector2d& vec) {
        return point(x-vec.x, y-vec.y);
    }
    point& operator -= (const vector2d& vec) {
        return *this = *this - vec;
    }

    vector2d operator - (const point& pt) {
        return vector2d(x-pt.x,y-pt.y);
    }
};

#endif // __POINT_H
