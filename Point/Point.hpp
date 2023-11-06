#pragma once
#include <iostream>

struct Point {
    double x, y, z;

    Point(double = 0, double = 0, double = 0);

    Point cross(const Point&);
    double dot(const Point&) const;
    Point normalize();
    double mag();

    Point operator+(const Point&) const;
    Point operator+=(const Point&);
    Point operator-(const Point&) const;
    Point operator-=(const Point&);
    Point operator*(const double) const;
    Point operator=(const Point&);
    friend std::ostream& operator<<(std::ostream&, const Point&);
};