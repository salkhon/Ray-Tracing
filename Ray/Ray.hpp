#pragma once
#include "../Point/Point.hpp"

struct Ray {
    Point start;
    Point direction;

    Ray(Point start, Point direction);

    Point reflected_dir(const Point& normal) const;
    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);
};