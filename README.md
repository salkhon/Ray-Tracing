# Ray Tracing Using OpenGL

## Running the program

1. Install opengl and glut

```bash
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

2. Run the program using the makefile

```bash
make run
```

## Controls

| Key | Control |
| --- | --- |
| W   | Move forward |
| S   | Move backward |
| A   | Move left |
| D   | Move right |
| UP   | Move camera up |
| DOWN   | Move camera down |
| LEFT   | Move camera left |
| RIGHT   | Move camera right |
| 5   | Rotate camera clockwise |
| 6   | Rotate camera counter-clockwise |
| PGUP   | Increase height |
| PGDOWN   | Decrease height |
| C | Capture frame for Ray Tracing |

## Notes

- If `C` is pressed, the current OpenGL frame will be captured and the `RAY-TRACING` algorithm will run on that frame to produce the image `output.bmp`.
- OpenGL objects and their properties can be changed in `spec/description.txt` file.
