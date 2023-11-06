#pragma once
#include "../Point/Point.hpp"
#include "../PointLight/PointLight.hpp"
#define RAD2DEG(rad) (rad * 180.0 / M_PI)

struct SpotLight {
    PointLight pointlight;
    Point direction;
    double cutoff_angle;

    SpotLight(Point source, double falloff, Point direction, double cutoff_angle);
    void draw();

    friend std::ostream& operator<<(std::ostream& os, const SpotLight& l);
};