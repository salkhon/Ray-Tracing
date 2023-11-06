#pragma once
#include <vector>
#include <memory>
#include <cmath>
#include "PointLight/PointLight.hpp"
#include "SpotLight/SpotLight.hpp"

struct Object;
using ObjectPtrs = std::vector<std::shared_ptr<Object>>;

struct Config {
    double near, far, fov_y, asr;
    double win_width, win_height;
    size_t recur_level, n_pix;

    Point eye = { 200, 200, 100 };
    Point look_dir = { -1 / sqrt(2), -1 / sqrt(2), 0 };
    Point right_dir = { -1 / sqrt(2), 1 / sqrt(2), 0 };
    Point up_dir = { 0, 0, 1 };

    ObjectPtrs objects;
    std::vector<PointLight> pointlights;
    std::vector<SpotLight> spotlights;
    bool is_texture = false;
};

extern Config cfg;