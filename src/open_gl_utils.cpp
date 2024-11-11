#include "cube.hpp"
#include <open_gl_utils.hpp>

float angleX = 0.0f,
      angleY = 0.0f;        // Cube rotation angles for X and Y axes
int lastMouseX, lastMouseY; // To track the last mouse position
bool mouseDragging = false; // To check if the mouse is being dragged
// Function to render bitmap text
void open_gl_utils::renderText(float x, float y, float z,
                               const std::string &text) {
  glRasterPos3f(x, y, z);     // Set the position for text
  glScalef(1.5f, 1.5f, 1.0f); // Scale the text (increase size)

  for (char c : text) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Draw each character
  }
}

// Initialize OpenGL settings
void open_gl_utils::init() {
  glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
  glEnable(GL_DEPTH_TEST);          // Enable depth testing
  std::unordered_set<int> existingValues;
  cube::initCube(existingValues); // Initialize the cube
}

// Display function to render the cube
void open_gl_utils::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
  glLoadIdentity();

  // Set the camera position
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // Apply rotation based on mouse drag
  glRotatef(angleX, 1.0, 0.0, 0.0); // Rotate around X-axis
  glRotatef(angleY, 0.0, 1.0, 0.0); // Rotate around Y-axis

  // Draw cubes and numbers
  float cubeSize = 1.0f; // Size of each sub-cube
  for (int i = 0; i < cube::N; ++i) {
    for (int j = 0; j < cube::N; ++j) {
      for (int k = 0; k < cube::N; ++k) {

        glPushMatrix();
        glTranslatef(i * cubeSize - cube::N / 2.0f,
                     j * cubeSize - cube::N / 2.0f,
                     k * cubeSize - cube::N / 2.0f);

        // Draw wireframe cube with constant color (white)
        glColor3f(0.5f, 0.5f, 0.5f); // Gray frame color
        glutWireCube(cubeSize);

        // Change color of the numbers based on the layer (Z-axis)
        if (k == 0) {
          glColor3f(0.15686f, 0.45490f, 0.65176f);
        } else if (k == 1) {
          glColor3f(0.18039f, 0.8f, 0.44314f);
        } else if (k == 2) {
          glColor3f(244.0f / 255.0f, 208.0f / 255.0f,
                    63.0f / 255.0f); // Sets the color to RGB (244, 208, 63)
        } else if (k == 3) {
          glColor3f(0.90784f, 0.29804f, 0.23529f); // Set to a vibrant red color
        } else if (k == 4) {
          glColor3f(0.54118f, 0.60392f, 0.35686f);
        }

        // Convert the number to string
        std::stringstream ss;
        ss << cube::cube[i][j][k];
        std::string numStr = ss.str();

        // Calculate the width of the text (number) to center it
        float textWidth = numStr.length() *
                          0.15f; // Adjust 0.15 to control text width scaling
        renderText(
            -textWidth / 2.0f, 0.0f, 0.0f,
            numStr); // Centered horizontally and at the middle vertically
        glPopMatrix();
      }
    }
  }

  glutSwapBuffers(); // Swap buffers for smooth animation
}

// Reshape function to adjust the viewport
void open_gl_utils::reshape(int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
  glMatrixMode(GL_MODELVIEW);
}

// Mouse button event handler
void open_gl_utils::mouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      // Start dragging
      mouseDragging = true;
      lastMouseX = x;
      lastMouseY = y;
    } else if (state == GLUT_UP) {
      // Stop dragging
      mouseDragging = false;
    }
  }
}

// Mouse motion event handler (when dragging)
void open_gl_utils::mouseMotion(int x, int y) {
  if (mouseDragging) {
    // Calculate how much the mouse moved
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    // Update rotation angles based on the mouse movement
    angleX += deltaY * 0.5f; // Rotate around X-axis
    angleY += deltaX * 0.5f; // Rotate around Y-axis

    lastMouseX = x; // Update the last mouse position
    lastMouseY = y;

    // Request a redraw
    glutPostRedisplay();
  }
}
