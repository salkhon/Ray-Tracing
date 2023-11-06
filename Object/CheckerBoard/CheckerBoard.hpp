#pragma once
#include "../Object.hpp"
#include "../../bitmap_image.hpp"

struct CheckerBoard : public Object {
    inline const static int HORIZON = 100;
    bitmap_image texture_b, texture_w;

    CheckerBoard(double w, double ambient, double diffuse, double reflection);

    void draw() override;
    double intersect(const Ray& ray) override;
    Point normal(const Point& point) override;

    Color color_at(Point& point) override;

    friend std::ostream& operator<<(std::ostream& os, const CheckerBoard& cb);
};