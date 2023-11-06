#include "Ray.hpp"

Ray::Ray(Point start, Point direction) :
    start{ start }, direction{ direction } {
}

Point Ray::reflected_dir(const Point& normal) const {
    return { this->direction - normal * 2 * this->direction.dot(normal) };
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    return os << "<Ray start=" << ray.start << " direction=" << ray.direction << ">";
}
