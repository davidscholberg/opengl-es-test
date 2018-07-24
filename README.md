This is a test repo for learning various OpenGL and C++ concepts.

You'll need the following dependencies to build this project:

* CMake
* A C++ compiler (e.g. `g++`, `clang`, etc.)
* SDL2 development packages
* OpenGL ES 2.0 development packages

To install the dependencies on Ubuntu/Debian:

```bash
apt install cmake g++ libsdl2-2.0-0 libsdl2-dev libgles2-mesa libgles2-mesa-dev
```

To build this project, navigate to the `build/` directory under the repo root and run:

```bash
cmake ..
make
```

This will generate a binary named `opengl-es-test`. Run it without any options to get usage information.

## Resources

* http://opengl.datenwolf.net/gltut/html/index.html
* https://learnopengl.com/
