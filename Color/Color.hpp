#pragma once
#include <iostream>

struct Color {
    double r, g, b;

    Color normalize();

    Color operator*(const double factor) const;
    Color operator+(const Color& other) const;
    Color operator+=(const Color& other);
    friend std::ostream& operator<<(std::ostream& os, const Color& color);
};