#include "PointLight.hpp"
#include "GL/glut.h"

PointLight::PointLight(Point point, double falloff) :
    point{ point }, falloff{ falloff } {
}

void PointLight::draw() {
    glColor3d(1, 1, 1);
    glPushMatrix(); {
        glTranslated(this->point.x, this->point.y, this->point.z);
        glutSolidTorus(1, 5, 10, 10);
    } glPopMatrix();
}

std::ostream& operator<<(std::ostream& os, const PointLight& l) {
    return os << "<PointLight point=" << l.point << " falloff=" << l.falloff << ">";
}
