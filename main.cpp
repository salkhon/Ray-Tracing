#include <GL/glut.h>
#include <cmath>
#include "Camera/Camera.hpp"
#include <fstream>
#include "Object/Object.hpp"
#include "Object/Sphere/Sphere.hpp"
#include "Object/CheckerBoard/CheckerBoard.hpp"
#include "PointLight/PointLight.hpp"
#include "SpotLight/SpotLight.hpp"
#include "Config.hpp"
#include "decompose.hpp"

#define DEG2RAD(deg) ((deg) * M_PI / 180.0)

// globals
Config cfg;
const double AXES_LEN = 100;

// camera variables
Camera camera(cfg);

// callbacks
void display();
void idle();
void specialKeyListener(int, int, int);
void keyboardListener(unsigned char, int, int);

// utils
void drawAxes();
void printObjects();

// initialize values
void initGL() {
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cfg.fov_y, cfg.asr, cfg.near, cfg.far);
}

void loadConfigData(std::string filename) {
    std::fstream input(filename);
    input >> cfg.near >> cfg.far >> cfg.fov_y >> cfg.asr;

    // window height and width in coord sys units
    cfg.win_height = 2 * cfg.near * tan(DEG2RAD(cfg.fov_y / 2));
    cfg.win_width = 2 * cfg.near * tan(DEG2RAD(cfg.fov_y * cfg.asr / 2));

    input >> cfg.recur_level >> cfg.n_pix;

    double checker_w, checker_ambient, checker_diffuse, checker_reflection;
    input >> checker_w >> checker_ambient >> checker_diffuse >> checker_reflection;

    // first object is checkerboard
    cfg.objects.push_back(std::make_shared<CheckerBoard>(
        checker_w, checker_ambient, checker_diffuse, checker_reflection));

    size_t n_obj;
    input >> n_obj;

    std::string objtype;
    Point ref_point;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;

    for (size_t i = 0; i < n_obj; i++) {
        input >> objtype;
        input >> ref_point.x >> ref_point.y >> ref_point.z;
        if (objtype == "sphere") {
            double rad;
            input >> rad;
            input >> color.r >> color.g >> color.b;
            input >> ambient >> diffuse >> specular >> reflection >> shine;
            cfg.objects.push_back(std::make_shared<Sphere>(
                ref_point, rad, color, ambient, diffuse, specular, reflection, shine));
        } else if (objtype == "pyramid") {
            double w, h;
            input >> w >> h;
            input >> color.r >> color.g >> color.b;
            input >> ambient >> diffuse >> specular >> reflection >> shine;
            auto triangle_ptrs = decomponse_pyramid(ref_point, w, h, color, ambient, diffuse, specular, reflection, shine);
            cfg.objects.insert(cfg.objects.end(), triangle_ptrs.begin(), triangle_ptrs.end());
        } else if (objtype == "cube") {
            double side;
            input >> side;
            input >> color.r >> color.g >> color.b;
            input >> ambient >> diffuse >> specular >> reflection >> shine;
            auto triangle_ptrs = decompose_cube(ref_point, side, color, ambient, diffuse, specular, reflection, shine);
            cfg.objects.insert(cfg.objects.end(), triangle_ptrs.begin(), triangle_ptrs.end());
        }
    }

    size_t n_pointlights;
    input >> n_pointlights;

    Point src;
    double falloff;
    for (size_t i = 0; i < n_pointlights; i++) {
        input >> src.x >> src.y >> src.z >> falloff;
        cfg.pointlights.push_back({ src, falloff });
    }

    size_t n_spotlights;
    input >> n_spotlights;

    Point direction;
    double cutoff_angle;
    for (size_t i = 0; i < n_spotlights; i++) {
        input >> src.x >> src.y >> src.z >> falloff;
        input >> direction.x >> direction.y >> direction.z;
        input >> cutoff_angle;
        cfg.spotlights.push_back({ src, falloff, direction, cutoff_angle });
    }
}

int main(int argc, char* argv[]) {
    loadConfigData(argv[1]);

    glutInit(&argc, argv);
    // window width and height are being set in coord sys units
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("RTX");

    initGL();

    glEnable(GL_DEPTH_TEST);  // enable Depth Testing

    glutDisplayFunc(display);  // display callback function
    glutIdleFunc(idle);  // what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();  // The main loop of OpenGL
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,
        camera.eye.x + camera.look_dir.x, camera.eye.y + camera.look_dir.y, camera.eye.z + camera.look_dir.z,
        camera.up_dir.x, camera.up_dir.y, camera.up_dir.z);

    // draw
    drawAxes();
    for (auto obj : cfg.objects) {
        obj->draw();
    }
    for (auto pointlight : cfg.pointlights) {
        pointlight.draw();
    }
    for (auto spotlight : cfg.spotlights) {
        spotlight.draw();
    }

    glutSwapBuffers(); // Render now
}

void idle() {
    glutPostRedisplay();
}

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES); {
        glColor3f(1, 0, 0);  // Red
        glVertex3f(0, 0, 0);
        glVertex3f(AXES_LEN, 0, 0);

        glColor3f(0, 1, 0);  // Green
        glVertex3f(0, 0, 0);
        glVertex3f(0, AXES_LEN, 0);

        glColor3f(0, 0, 1);  // Blue
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, AXES_LEN);
    } glEnd();
}

void keyboardListener(unsigned char key, int mouse_x, int mouse_y) {
    switch (key) {
    case '1':
    case 'a':
        // rotate left
        camera.rotate(Camera::ANG_VELOCITY);
        break;
    case '2':
    case 'd':
        // rotate right
        camera.rotate(-Camera::ANG_VELOCITY);
        break;
    case '3':
    case 'w':
        // rotate up
        camera.rotate(0, Camera::ANG_VELOCITY);
        break;
    case '4':
    case 's':
        // rotate down
        camera.rotate(0, -Camera::ANG_VELOCITY);
        break;
    case '5':
    case 'e':
        // tilt clockwise
        camera.rotate(0, 0, Camera::ANG_VELOCITY);
        break;
    case '6':
    case 'q':
        // tilt counter clockwise
        camera.rotate(0, 0, -Camera::ANG_VELOCITY);
        break;
    case '0':
    case 'c':
        camera.capture();
        break;
    case ' ':
        cfg.is_texture = !cfg.is_texture;
        std::cout << "texture enabled: " << cfg.is_texture << std::endl;
    default:
        break;
    }

    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        // move forward
        camera.translate(camera.look_dir * Camera::VELOCITY);
        break;
    case GLUT_KEY_DOWN:
        // move backward
        camera.translate(camera.look_dir * -Camera::VELOCITY);
        break;
    case GLUT_KEY_RIGHT:
        // move right
        camera.translate(camera.right_dir * Camera::VELOCITY);
        break;
    case GLUT_KEY_LEFT:
        // move left
        camera.translate(camera.right_dir * -Camera::VELOCITY);
        break;
    case GLUT_KEY_PAGE_UP:
        // move up
        camera.translate(camera.up_dir * Camera::VELOCITY);
        break;
    case GLUT_KEY_PAGE_DOWN:
        // move down
        camera.translate(camera.up_dir * -Camera::VELOCITY);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void printObjects() {
    for (auto objptr : cfg.objects) {
        std::cout << *objptr << std::endl;
    }
}