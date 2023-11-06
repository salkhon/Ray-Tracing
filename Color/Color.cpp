#include "Color.hpp"
#include "../Object/Object.hpp"

std::ostream& operator<<(std::ostream& os, const Object& obj) {
    return os << "<Object type=" << obj.type << " ref=" << obj.ref_point
        << " lwh=(" << obj.l << "," << obj.w << "," << obj.h << ") col=" << obj.color
        << " amb=" << obj.ambient << " diff=" << obj.diffuse << " spec=" << obj.specular
        << " refl=" << obj.reflection << " shine=" << obj.shine << ">";
}

std::ostream& operator<<(std::ostream& os, const Color& color) {
    return os << "<Color (" << color.r << "," << color.g << "," << color.b << ")>";
}

Color Color::normalize() {
    double r = std::min(std::max(0.0, this->r), 1.0);
    double g = std::min(std::max(0.0, this->g), 1.0);
    double b = std::min(std::max(0.0, this->b), 1.0);
    return { r, g, b };
}

Color Color::operator*(const double factor) const {
    return { this->r * factor, this->g * factor, this->b * factor };
}

Color Color::operator+(const Color& other) const {
    return { this->r + other.r, this->g + other.g, this->b + other.b };
}

Color Color::operator+=(const Color& other) {
    *this = *this + other;
    return *this;
}