#pragma once
#include "../Object.hpp"

struct Sphere : public Object {
    inline const static int STACKS = 100, SLICES = 100;
    
    Sphere(Point center, double rad,
        Color color, double ambient_coeff, double diffuse_coeff,
        double specular_coeff, double reflection_coeff, int shine);

    void draw() override;
    double intersect(const Ray& ray) override;
    Point normal(const Point& point) override;

    friend std::ostream& operator<<(std::ostream& os, const Sphere& sphere);
};