#include "Point.hpp"
#include <cmath>

Point::Point(double x, double y, double z) : x{ x }, y{ y }, z{ z } {
}

Point Point::cross(const Point& other) {
    return  { this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x };
}

double Point::dot(const Point& other) const {
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

Point Point::normalize() {
    double mag = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    return { this->x / mag, this->y / mag, this->z / mag };
}

double Point::mag() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Point Point::operator+(const Point& other) const {
    return { this->x + other.x, this->y + other.y, this->z + other.z };
}

Point Point::operator+=(const Point& other) {
    *this = *this + other;
    return *this;
}

Point Point::operator-(const Point& other) const {
    return { this->x - other.x, this->y - other.y, this->z - other.z };
}

Point Point::operator-=(const Point& other) {
    *this = *this - other;
    return *this;
}

Point Point::operator*(const double factor) const {
    return { this->x * factor, this->y * factor, this->z * factor };
}

Point Point::operator=(const Point& other) {
    this->x = other.x, this->y = other.y, this->z = other.z;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "<Point(" << p.x << "," << p.y << "," << p.z << ")>";
}
