#ifndef open_gl_utils_hpp
#define open_gl_utils_hpp

#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <cube.hpp>

namespace open_gl_utils {

// Function to render bitmap text
void renderText(float x, float y, float z, const std::string &text);

// Initialize the cube with unique and random values (1 to N^5)
void initCube(std::unordered_set<int> existingValues);

// Initialize OpenGL settings
void init();

// Display function to render the cube
void display();

// Reshape function to adjust the viewport
void reshape(int w, int h);

// Mouse button event handler
void mouseButton(int button, int state, int x, int y);

// Mouse motion event handler (when dragging)
void mouseMotion(int x, int y);

} // namespace open_gl_utils

#endif
