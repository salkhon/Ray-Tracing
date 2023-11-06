#pragma once
#include <vector>
#include <memory>
#include "Object/Triangle/Triangle.hpp"
#include "Point/Point.hpp"
#include "Object/Object.hpp"

using TrianglePtrs = std::vector<std::shared_ptr<Triangle>>;

TrianglePtrs decomponse_pyramid(Point lowest_point, double width, double height, Color color,
    double ambient, double diffuse, double specular, double reflection, double shine) {
    TrianglePtrs triangle_ptrs;
    Point a, b, c, d;
    a = b = c = d = lowest_point;
    // bottom quad
    b.y += width;
    c.y += width;
    c.x += width;
    d.x += width;
    triangle_ptrs.push_back(std::make_shared<Triangle>(a, b, c, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(c, d, a, color, ambient, diffuse, specular, reflection, shine));

    // top triangles
    // diagonal intersection
    Point e = a + (c - a) * 0.5;
    e.z += height;
    // rear
    triangle_ptrs.push_back(std::make_shared<Triangle>(e, b, a, color, ambient, diffuse, specular, reflection, shine));
    // left
    triangle_ptrs.push_back(std::make_shared<Triangle>(e, a, d, color, ambient, diffuse, specular, reflection, shine));
    // front
    triangle_ptrs.push_back(std::make_shared<Triangle>(e, d, c, color, ambient, diffuse, specular, reflection, shine));
    // right
    triangle_ptrs.push_back(std::make_shared<Triangle>(e, c, b, color, ambient, diffuse, specular, reflection, shine));

    return triangle_ptrs;
}

TrianglePtrs decompose_cube(Point bottom_lower_left_point, double side, Color color,
    double ambient, double diffuse, double specular, double reflection, double shine) {
    TrianglePtrs triangle_ptrs;
    Point a, b, c, d, e, f, g, h;
    a = b = c = d = e = f = g = h = bottom_lower_left_point;

    d.x += side;
    b.y += side;
    e.z += side;

    c.x += side, c.y += side;
    h.x += side, h.z += side;
    f.y += side, f.z += side;

    g.x += side, g.y += side, g.z += side;

    // bottom
    triangle_ptrs.push_back(std::make_shared<Triangle>(a, b, c, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(c, d, a, color, ambient, diffuse, specular, reflection, shine));

    // top
    triangle_ptrs.push_back(std::make_shared<Triangle>(e, h, g, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(g, f, e, color, ambient, diffuse, specular, reflection, shine));

    // left
    triangle_ptrs.push_back(std::make_shared<Triangle>(e, a, d, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(d, h, e, color, ambient, diffuse, specular, reflection, shine));

    // front
    triangle_ptrs.push_back(std::make_shared<Triangle>(g, h, d, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(d, c, g, color, ambient, diffuse, specular, reflection, shine));

    // right
    triangle_ptrs.push_back(std::make_shared<Triangle>(f, g, c, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(c, b, f, color, ambient, diffuse, specular, reflection, shine));

    // rear
    triangle_ptrs.push_back(std::make_shared<Triangle>(a, e, f, color, ambient, diffuse, specular, reflection, shine));
    triangle_ptrs.push_back(std::make_shared<Triangle>(f, b, a, color, ambient, diffuse, specular, reflection, shine));

    return triangle_ptrs;
}