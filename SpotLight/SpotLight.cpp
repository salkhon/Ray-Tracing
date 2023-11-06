#include "SpotLight.hpp"
#include "GL/glut.h"
#include <cmath>

SpotLight::SpotLight(Point source, double falloff, Point direction, double cutoff_angle) :
    pointlight(source, falloff), direction{ direction.normalize() }, cutoff_angle{ cutoff_angle } {
}

void SpotLight::draw() {
    double angle = acos(this->direction.dot({ 0, 0, -1 }));
    Point rot_axis = this->direction.cross({ 0, 0, -1 }).normalize();
    glColor3d(0.5, 0.5, 0.5);
    glPushMatrix(); {
        glTranslated(this->pointlight.point.x, this->pointlight.point.y, this->pointlight.point.z);
        glBegin(GL_LINES); {
            glVertex3d(0, 0, 0);
            glVertex3d(this->direction.x * 30, this->direction.y * 30, this->direction.z * 30);
        } glEnd();

        glRotated(-RAD2DEG(angle), rot_axis.x, rot_axis.y, rot_axis.z);
        glutSolidCone(5, 10, 50, 50);
    } glPopMatrix();
}

std::ostream& operator<<(std::ostream& os, const SpotLight& l) {
    return os << "<SpotLight pointlight=" << l.pointlight << " direction=" << l.direction
        << " cutoff_angle=" << l.cutoff_angle << ">";
}
