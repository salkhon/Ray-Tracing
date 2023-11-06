#include "Sphere.hpp"
#include <vector>
#include <cmath>
#include <GL/glut.h>

Sphere::Sphere(Point center, double rad,
    Color color, double ambient, double diffuse,
    double specular, double reflection, int shine) :
    Object("sphere", center, rad, rad, rad, color, ambient, diffuse, specular, reflection, shine) {
}

void Sphere::draw() {
    const Point center = this->ref_point;
    const double rad = this->l;
    // todo: use glutSolidSphere()
    std::vector<std::vector<Point>> points(STACKS + 1, std::vector<Point>(SLICES + 1));
    for (size_t i = 0; i <= STACKS; i++) {
        double phi = -M_PI / 2.0 + i * M_PI / STACKS;
        double r = rad * cos(phi);
        double h = rad * sin(phi);
        for (size_t j = 0; j <= SLICES; j++) {
            double theta = j * 2 * M_PI / SLICES;
            points[i][j].x = r * cos(theta);
            points[i][j].y = r * sin(theta);
            points[i][j].z = h;
        }
    }

    glPushMatrix(); {
        glColor3d(this->color.r, this->color.g, this->color.b);
        glTranslated(center.x, center.y, center.z);

        glBegin(GL_QUADS); {
            for (size_t i = 0; i < STACKS; i++) {
                for (size_t j = 0; j < SLICES; j++) {
                    glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3d(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3d(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                }
            }

        } glEnd();
    } glPopMatrix();
}

double Sphere::intersect(const Ray& ray) {
    Point center = this->ref_point;
    double rad = this->l;

    auto sphere_origin = ray.start - center;

    double a = 1;
    double b = (ray.direction.dot(sphere_origin)) * 2;
    double c = (sphere_origin.dot(sphere_origin)) - rad * rad;

    double disc = b * b - 4 * a * c;
    if (disc < 0) {
        return -1;
    } else if (std::abs(a) < 1e-5) {
        return -c / b;
    }

    disc = sqrt(disc);
    double t1 = (-b - disc) / (2 * a), t2 = (-b + disc) / (2 * a);
    if (t1 > t2) std::swap(t1, t2);

    if (t1 < 0 && t2 < 0) {
        return -1;
    } else if (t1 > 0) {
        // return closest t first
        return t1;
    } else if (t2 > 0) {
        return t2;
    } else {
        return -1;
    }
}

Point Sphere::normal(const Point& point) {
    return (point - this->ref_point).normalize();
}

std::ostream& operator<<(std::ostream& os, const Sphere& sphere) {
    return os << "<Sphere obj=" << (Object&)sphere << ">";
}
