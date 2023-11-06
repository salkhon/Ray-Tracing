#include "Camera.hpp"
#include "../Ray/Ray.hpp"
#include "../Object/Object.hpp"
#include <memory>

Camera::Camera(Config& cfg,
    const Point& eye, const Point& look_dir, const Point& right_dir, const Point& up_dir) :
    cfg{ cfg }, eye{ eye }, look_dir{ look_dir }, right_dir{ right_dir }, up_dir{ up_dir } {

}

void Camera::translate(const Point& translate_dir) {
    this->eye += translate_dir;
}

void _rotate(Point& rotatee, Point& axis, const double angle) {
    rotatee = rotatee * cos(angle) + (axis.cross(rotatee) * sin(angle));
}

/**
 * @brief Rotate the camera along it's axes.
 *
 * @param left_angle Angle to rotate look direction leftward
 * @param up_angle Angle to rotate look direction upward
 * @param tilt_angle Angle to tilt camera clockwise
 */
void Camera::rotate(const double left_angle, const double up_angle, const double tilt_angle) {
    if (left_angle != 0) {
        // rotating right vec towards look vec (u cross r)
        _rotate(this->right_dir, this->up_dir, left_angle);
        // rotating look vec towards left vec (u cross l)
        _rotate(this->look_dir, this->up_dir, left_angle);
    }
    if (up_angle != 0) {
        // rotating up vec towards back vec (r cross u)
        _rotate(this->up_dir, this->right_dir, up_angle);
        // rotating look vec towards up vec (r cross l)
        _rotate(this->look_dir, this->right_dir, up_angle);
    }
    if (tilt_angle != 0) {
        // rotating right vec towards down vec (l cross r)
        _rotate(this->right_dir, this->look_dir, tilt_angle);
        // rotating up vec towards right vec (l cross u)
        _rotate(this->up_dir, this->look_dir, tilt_angle);
    }
}

void Camera::capture() {
    std::cout << "Capturing image..." << std::endl;
    bmp_img = bitmap_image(cfg.n_pix, cfg.n_pix);

    // init black image
    for (size_t x = 0; x < cfg.n_pix; x++) {
        for (size_t y = 0; y < cfg.n_pix; y++) {
            bmp_img.set_pixel(x, y, 0, 0, 0);
        }
    }

    // topleft point coordinate of the near plane (dynamic based on camera position and orientation)
    Point topleft = this->eye
        + this->look_dir * cfg.near
        - this->right_dir * (cfg.win_width / 2.0)
        + this->up_dir * (cfg.win_height / 2.0);

    // image pixel dimensions in coord system units
    double pixw = cfg.win_width / cfg.n_pix;
    double pixh = cfg.win_height / cfg.n_pix;

    // correcting topleft point to be in the middle of the pixel grid
    topleft = topleft + this->right_dir * (0.5 * pixw) - this->up_dir * (0.5 * pixh);

    // generate rays for each pixel grid position (point buffer grid cell)
    for (size_t x = 0; x < cfg.n_pix; x++) {
        for (size_t y = 0; y < cfg.n_pix; y++) {
            // current pixel/point buffer grid cell
            Point curr_pix = topleft + this->right_dir * x * pixw - this->up_dir * y * pixh;

            // cast ray from current pix towards (current pix - eye)
            Ray ray(curr_pix, (curr_pix - this->eye).normalize());
            auto nearest_obj_ptr = get_nearest_intersecting_obj(ray, cfg);

            // do the recursive intersection computation only if the ray hit an object
            Color color{ 0, 0, 0 }; // default black
            if (nearest_obj_ptr != nullptr) {
                color = nearest_obj_ptr->get_recursive_intersection_color(ray, 1, cfg);
            }

            color = color.normalize();
            bmp_img.set_pixel(x, y, color.r * 255, color.g * 255, color.b * 255);
        }
    }

    bmp_img.save_image("output.bmp");
    std::cout << "Image saved!" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Camera& cam) {
    return os << "< Camera eye=" << cam.eye << " look=" << cam.look_dir << " right=" << cam.right_dir
        << " up=" << cam.up_dir << " >";
}
