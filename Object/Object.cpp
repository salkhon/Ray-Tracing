#include "Object.hpp"
#include <algorithm>

Object::Object(std::string type, Point ref_point, double l, double w, double h,
    Color color, double ambient, double diffuse, double specular, double reflection, int shine) :
    type{ type }, ref_point{ ref_point }, l{ l }, w{ w }, h{ h },
    color{ color }, ambient{ ambient }, diffuse{ diffuse },
    specular{ specular }, reflection{ reflection }, shine{ shine } {
}

/* --------- HELPER FUNCTIONS ----------- */
bool is_obscured(Ray& ray, double t_obj, Config& cfg) {
    double t;
    // find if any other object intersects with this light ray closer than this object
    for (auto obj_ptr : cfg.objects) {
        t = obj_ptr->intersect(ray);
        if (t > 0 && t < t_obj) {
            return true;
        }
    }

    return false;
}

void update_ambient_color(Color& color, const Color& colot_at_intersection, double ambient) {
    color += colot_at_intersection * ambient;
}

void update_diffuse_color(Color& color, const Color& color_at_intersection, const Point& intersection,
    const Ray& lightray, double falloff, const Point& surface_normal, double diffuse) {
    double distance = (intersection - lightray.start).mag();
    double falloff_scaling = exp(distance * distance * falloff);
    double lambert = std::max(0.0, -lightray.direction.dot(surface_normal));
    color += color_at_intersection * diffuse * lambert;
}

void update_specular_color(Color& color, const Color& color_at_intersection, const Point& intersection,
    const Ray& lightray, double falloff, const Point& surface_normal, double specular, int shine) {
    double distance = (intersection - lightray.start).mag();
    double falloff_scaling = exp(distance * distance * falloff);
    Point reflected_ray_dir = lightray.reflected_dir(surface_normal);
    double phong = std::max(0.0, -lightray.direction.dot(reflected_ray_dir));
    color += color_at_intersection * specular * pow(phong, shine) * falloff_scaling;
}

std::shared_ptr<Object> get_nearest_intersecting_obj(const Ray& ray, const Config& cfg) {
    double t_min = INFINITY, t;
    std::shared_ptr<Object> nearest_obj_ptr = nullptr;
    for (auto obj_ptr : cfg.objects) {
        t = obj_ptr->intersect(ray);
        if (t > 0 && t < t_min) {
            t_min = t;
            nearest_obj_ptr = obj_ptr;
        }
    }
    return nearest_obj_ptr;
}
/*----------------------------------------*/

Color Object::get_recursive_intersection_color(const Ray& eye_ray, int recur_level, Config& cfg) {
    if (recur_level == 0) {
        return { 0, 0, 0 };
    }

    Color col{ 0, 0, 0 };
    double t_min = this->intersect(eye_ray);

    Point intersection = eye_ray.start + eye_ray.direction * t_min;
    Color color_at_intersection = this->color_at(intersection);

    update_ambient_color(col, color_at_intersection, ambient);

    // normalized normal
    Point normal = this->normal(intersection);

    /* POINT LIGHTS */
    for (auto pointlight : cfg.pointlights) {
        Ray lightray(pointlight.point, (intersection - pointlight.point).normalize());
        double t_obj = this->intersect(lightray); // t for light's intersection with this object

        if (!is_obscured(lightray, t_obj, cfg)) {
            update_diffuse_color(col, color_at_intersection, intersection, lightray, pointlight.falloff,
                normal, this->diffuse);

            update_specular_color(col, color_at_intersection, intersection, lightray, pointlight.falloff,
                normal, this->specular, this->shine);
        }
    }

    /* SPOT LIGHTS */
    for (auto spotlight : cfg.spotlights) {
        // ray from spotlight to current obj intersection
        Ray lightray(spotlight.pointlight.point, (intersection - spotlight.pointlight.point).normalize());

        // angle this ray makes with spotlight's direction (everything is normalized)
        // todo: rad2degree problem
        double created_angle = RAD2DEG(acos(lightray.direction.dot(spotlight.direction)));

        if (abs(created_angle) < spotlight.cutoff_angle) {
            // check if  lightray is obscured
            double t_obj = this->intersect(lightray);

            if (!is_obscured(lightray, t_obj, cfg)) {
                update_diffuse_color(col, color_at_intersection, intersection, lightray, spotlight.pointlight.falloff,
                    normal, this->diffuse);

                update_specular_color(col, color_at_intersection, intersection, lightray, spotlight.pointlight.falloff,
                    normal, this->specular, this->shine);
            }
        }
    }

    /* RECURSIVE REFECTIONS */
    if (recur_level < cfg.recur_level) {
        Ray reflected_ray(intersection, eye_ray.reflected_dir(normal));

        const double AVOID_SELF_INTERSECTION = 1e-5;
        reflected_ray.start += reflected_ray.direction * AVOID_SELF_INTERSECTION;

        auto nearest_intersecting_obj = get_nearest_intersecting_obj(reflected_ray, cfg);
        if (nearest_intersecting_obj != nullptr) {
            // reflected ray is the new eye ray for the recursive call
            Color reflection_col = nearest_intersecting_obj->get_recursive_intersection_color(
                reflected_ray, recur_level + 1, cfg);
            col += reflection_col * this->reflection;
        }
    }

    return col;
}

Color Object::color_at(Point& point) {
    return this->color;
}

