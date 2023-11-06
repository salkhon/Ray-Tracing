#include "GL/glut.h"
#include "Triangle.hpp"

Triangle::Triangle(Point a, Point b, Point c, Color color,
    double ambient, double diffuse, double specular, double reflection, int shine) :
    a{ a }, b{ b }, c{ c },
    Object("Triangle", a, 0, 0, 0, color, ambient, diffuse, specular, reflection, shine) {
}

void Triangle::draw() {
    glColor3d(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES); {
        glVertex3d(a.x, a.y, a.z);
        glVertex3d(b.x, b.y, b.z);
        glVertex3d(c.x, c.y, c.z);
    } glEnd();
}

double determinant(double mat[3][3]) {
    double v1 = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
    double v2 = mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
    double v3 = mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return v1 - v2 + v3;
}

double Triangle::intersect(const Ray& ray) {
    auto ab = this->a - this->b;
    auto ac = this->a - this->c;
    auto a_ro = this->a - ray.start;

    double beta_mat[3][3] = {
        {a_ro.x, ac.x, ray.direction.x},
        {a_ro.y, ac.y, ray.direction.y},
        {a_ro.z, ac.z, ray.direction.z},
    };
    double gamma_mat[3][3] = {
        {ab.x, a_ro.x, ray.direction.x},
        {ab.y, a_ro.y, ray.direction.y},
        {ab.z, a_ro.z, ray.direction.z},
    };
    double t_mat[3][3] = {
        {ab.x, ac.x, a_ro.x},
        {ab.y, ac.y, a_ro.y},
        {ab.z, ac.z, a_ro.z},
    };
    double a_mat[3][3] = {
        {ab.x, ac.x, ray.direction.x},
        {ab.y, ac.y, ray.direction.y},
        {ab.z, ac.z, ray.direction.z},
    };

    double a_det = determinant(a_mat);
    double beta = determinant(beta_mat) / a_det;
    double gamma = determinant(gamma_mat) / a_det;
    double t = determinant(t_mat) / a_det;

    if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0) {
        return t;
    } else {
        return -1;
    }
}

Point Triangle::normal(const Point& point) {
    return (this->b - this->a).cross(this->c - this->a).normalize();
}

std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
    return os << "<Triangle a=" << triangle.a << " b=" << triangle.b << " c=" << triangle.c << ">";
}
