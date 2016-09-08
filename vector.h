#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>

class vector2d {
public:
    vector2d() {};
    vector2d(double x, double y): x(x), y(y) {};
    virtual ~vector2d() {};

    vector2d(const vector2d &vec) {
        *this = vec;
    }

    vector2d& operator = (const vector2d &vec) {
        x = vec.x;
        y = vec.y;
        return *this;
    }

public:
    double x;
    double y;

public:
    double dot (vector2d &vec) {
        return x * vec.x + y * vec.y;
    }

    vector2d normalize() {
        return *this / length();
    }

    double length() {
        return sqrt(x*x + y*y);
    }

    double angle() {
        return atan2(y, x);
    }

public:
    vector2d operator + (const vector2d& vec) {
        return vector2d(x+vec.x, y+vec.y);
    }

    vector2d& operator += (const vector2d& vec) {
        return *this = *this + vec;
    }

    vector2d operator - () {
        return vector2d(-x, -y);
    }

    vector2d operator - (const vector2d& vec) {
        return vector2d(x-vec.x, y-vec.y);
    }

    vector2d& operator -= (const vector2d& vec) {
        return *this = *this - vec;
    }

    vector2d operator * (const double mul) {
        return vector2d(x*mul, y*mul);
    }

    vector2d& operator *= (const double mul) {
        return *this = *this * mul;
    }

    vector2d operator / (const double div) {
        return vector2d(x/div, y/div);
    }

    vector2d operator /= (const double div) {
        return *this = *this / div;
    }
};

#endif // __VECTOR_H
