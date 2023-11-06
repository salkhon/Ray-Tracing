#pragma once
#include "../Point/Point.hpp"

struct PointLight {
    Point point;
    double falloff;

    PointLight(Point point, double falloff);
    void draw();

    friend std::ostream& operator<<(std::ostream& os, const PointLight& l);
};