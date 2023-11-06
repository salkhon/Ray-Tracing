#pragma once
#include "../Point/Point.hpp"
#include "../Ray/Ray.hpp"
#include "../Config.hpp"
#include "../Color/Color.hpp"
#include <string>

struct Object {
    std::string type;
    Point ref_point;
    double l, w, h;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;

    Object(std::string type, Point ref_point, double l, double w, double h,
        Color color, double ambient, double diffuse,
        double specular, double reflection, int shine);

    // bounces ray recursively to nearby objects
    virtual Color get_recursive_intersection_color(const Ray& eye_ray, int recur_level, Config& cfg);
    virtual Color color_at(Point& point);

    // draw object with given info
    virtual void draw() = 0;
    // gives ray to object intersection parameter t (individual object)
    virtual double intersect(const Ray& ray) = 0;
    // gives normal of object at point (individual object)
    virtual Point normal(const Point& point) = 0;

    friend std::ostream& operator<<(std::ostream&, const Object&);
};

std::shared_ptr<Object> get_nearest_intersecting_obj(const Ray& ray, const Config& cfg);