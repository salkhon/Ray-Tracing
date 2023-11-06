#pragma once
#include "../../Point/Point.hpp"
#include "../Object.hpp"

struct Triangle : public Object {
    Point a, b, c;

    Triangle(Point a, Point b, Point c, Color color,
        double ambient, double diffuse, double specular, double reflection, int shine);

    void draw() override;
    double intersect(const Ray& ray) override;
    Point normal(const Point& point) override;

    friend std::ostream& operator<<(std::ostream& os, const Triangle& triangle);
};