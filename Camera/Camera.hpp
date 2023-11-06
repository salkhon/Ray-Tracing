#pragma once
#include "../Point/Point.hpp"
#include "../Config.hpp"
#include "../bitmap_image.hpp"
#include <cmath>

struct Camera {
    inline const static double VELOCITY = 4, ANG_VELOCITY = 0.03;

    Config& cfg;
    Point eye, look_dir, right_dir, up_dir;
    bitmap_image bmp_img;

    Camera(
        Config& cfg,
        const Point& eye = { 200, 200, 100 },
        // have to be unit vectors
        const Point& look_dir = { -1 / sqrt(2), -1 / sqrt(2), 0 },
        const Point& right_dir = { -1 / sqrt(2), 1 / sqrt(2), 0 },
        const Point& up_dir = { 0, 0, 1 });

    void translate(const Point&);
    void rotate(const double left_angle, const double up_angle = 0, const double tilt_angle = 0);

    void capture();

    friend std::ostream& operator<<(std::ostream&, const Camera&);
};