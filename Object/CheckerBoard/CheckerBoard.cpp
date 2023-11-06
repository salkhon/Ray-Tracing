#include "CheckerBoard.hpp"
#include "../../Config.hpp"
#include <GL/glut.h>

CheckerBoard::CheckerBoard(double w, double ambient, double diffuse, double reflection) :
    Object("checkerboard", { -HORIZON * w, -HORIZON * w, 0 }, w, w, w, { 0, 0, 0 },
        ambient, diffuse, 0, reflection, 0) {
    this->texture_b = bitmap_image("./texture_b.bmp");
    this->texture_w = bitmap_image("./texture_w.bmp");
}

void CheckerBoard::draw() {
    size_t col = 0;
    for (int i = -HORIZON; i < HORIZON; i++) {
        for (int j = -HORIZON; j < HORIZON; j++) {
            col = (i + j) % 2;
            glColor3d(col, col, col);
            glBegin(GL_QUADS); {
                glVertex3d(i * this->w, j * this->w, 0);
                glVertex3d((i + 1) * this->w, j * this->w, 0);
                glVertex3d((i + 1) * this->w, (j + 1) * this->w, 0);
                glVertex3d(i * this->w, (j + 1) * this->w, 0);
            } glEnd();
        }
    }
}

double CheckerBoard::intersect(const Ray& ray) {
    Point normal{ 0, 0, 1 };

    double t = -(normal.dot(ray.start)) / (normal.dot(ray.direction));
    Point intersection_point = ray.start + ray.direction * t;

    if (intersection_point.x < this->ref_point.x || intersection_point.x > -this->ref_point.x ||
        intersection_point.y < this->ref_point.y || intersection_point.y > -this->ref_point.y) {
        return -1;
    }

    return t;
}

Point CheckerBoard::normal(const Point& point) {
    return { 0, 0, 1 };
}

Color CheckerBoard::color_at(Point& point) {
    int row = (point.x - this->ref_point.x) / this->w;
    int col = (point.y - this->ref_point.y) / this->w;
    double c = (row + col) % 2;

    if (!cfg.is_texture) {
        return { c, c, c };
    }

    const double TILE_PERCENT = 0.5, TEX_PERCENT = 1 - TILE_PERCENT;

    // getting the relative position of the point within the tile
    Point relative = (point - this->ref_point);
    relative.x -= this->w * row, relative.y -= this->w * col;

    bitmap_image* texture = &(c == 0 ? this->texture_b : this->texture_w);
    relative.x *= texture->height() / this->w, relative.y *= texture->width() / this->w;
    unsigned texx = relative.x, texy = relative.y;
    unsigned char r, g, b;
    texture->get_pixel(texx, texy, r, g, b);

    return { c * TILE_PERCENT + r / 255.0 * TEX_PERCENT,
        c * TILE_PERCENT + g / 255.0 * TEX_PERCENT,
        c * TILE_PERCENT + b / 255.0 * TEX_PERCENT };
}

std::ostream& operator<<(std::ostream& os, const CheckerBoard& cb) {
    return os << "<CheckerBoard obj=" << (Object&)cb << ">";
}

