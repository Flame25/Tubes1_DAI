#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <thread>
#include <unordered_set>

const int num_neighbors = 10;       // Number of neighbors to generate
const int N = 5;                    // Cube size (5x5x5)
int cube[N][N][N];                  // 3D array to represent the cube
float angleX = 0.0f, angleY = 0.0f; // Cube rotation angles for X and Y axes
int lastMouseX, lastMouseY;         // To track the last mouse position
bool mouseDragging = false;         // To check if the mouse is being dragged

struct post {
  int x = 0;
  int y = 0;
  int z = 0;
};

// ==== OpenGL Function ====

// Function to render bitmap text
void renderText(float x, float y, float z, const std::string &text) {
  glRasterPos3f(x, y, z);     // Set the position for text
  glScalef(1.5f, 1.5f, 1.0f); // Scale the text (increase size)

  for (char c : text) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Draw each character
  }
}

// Display per Layer of Cube via CMD
void displayCube() {
  for (int i = 0; i < N; ++i) {
    std::cout << "Layer " << i + 1 << ":\n";
    for (int j = N - 1; j >= 0; --j) {
      for (int k = 0; k < N; ++k) {
        std::cout << cube[i][j][k] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
}

// Initialize the cube with unique and random values (1 to N^5)
void initCube(std::unordered_set<int> existingValues) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        int randomValue;
        do {
          randomValue =
              rand() % 125 + 1; // Generates a number between 1 and 125
        } while (existingValues.find(randomValue) !=
                 existingValues.end()); // Ensure uniqueness

        // Add the value to the set of existing values
        existingValues.insert(randomValue);

        // Assign the unique value to the cube at position (i, j, k)
        cube[i][j][k] = randomValue;
      }
    }
  }
  displayCube();
}

// Initialize OpenGL settings
void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
  glEnable(GL_DEPTH_TEST);          // Enable depth testing
  std::unordered_set<int> existingValues;
  initCube(existingValues); // Initialize the cube
}

// Display function to render the cube
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
  glLoadIdentity();

  // Set the camera position
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // Apply rotation based on mouse drag
  glRotatef(angleX, 1.0, 0.0, 0.0); // Rotate around X-axis
  glRotatef(angleY, 0.0, 1.0, 0.0); // Rotate around Y-axis

  // Draw cubes and numbers
  float cubeSize = 1.0f; // Size of each sub-cube
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {

        glPushMatrix();
        glTranslatef(i * cubeSize - N / 2.0f, j * cubeSize - N / 2.0f,
                     k * cubeSize - N / 2.0f);

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
        ss << cube[i][j][k];
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
void reshape(int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
  glMatrixMode(GL_MODELVIEW);
}

// Mouse button event handler
void mouseButton(int button, int state, int x, int y) {
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
void mouseMotion(int x, int y) {
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

// Change all number to 0 but periodic
void testUpdateCube() {
  auto start = std::chrono::steady_clock::now(); // Start time tracking
  std::chrono::milliseconds totalWaitTime(
      5000); // Total time to wait (5 seconds)

  int i = 0, j = 0, k = 0;

  while (true) {
    // Check how much time has passed
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

    // If elapsed time exceeds the total wait time, update and reset
    if (elapsed >= totalWaitTime) {
      std::cout << "Updating cube[" << i << "][" << j << "][" << k << "]\n";
      cube[i][j][k] = 0; // Update cube value

      k++; // Move to the next k index

      if (k == N) {
        k = 0; // Reset k and increment j
        j++;
      }

      if (j == N) {
        j = 0; // Reset j and increment i
        i++;
      }

      if (i == N) {
        std::cout << "Cube update completed.\n";
        return; // Exit the function when the entire cube is updated
      }

      // Reset the start time after each update
      start = std::chrono::steady_clock::now();
    }
  }
}

// ==== Hill Climbing Function & Utils ====
int objective_func(int magic_number, int cube[N][N][N]) {
  int error = 0;
  for (int i = 0; i < N; i++) {
    int sumRow = 0, sumColumn = 0, sumDiagonal = 0, sumPillar = 0;
    for (int j = 0; j < N; j++) {
    }
  }
  return error;
}

void swap(int temp_cube[N][N][N], int x1, int y1, int z1, int x2, int y2,
          int z2) {
  int temp = temp_cube[x1][y1][z1];
  temp_cube[x1][y1][z1] = temp_cube[x2][y2][z2];
  temp_cube[x2][y2][z2] = temp;
}

void copy_cube(int new_cube[N][N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        new_cube[i][j][k] = cube[i][j][k];
      }
    }
  }
}

int cube_func(int i, int j, int k) {
  int min_val = -999;
  for (int a = 0; a < N; a++) {
    swap(cube, i, j, k, 0, 0, a);
    int temp_err1 = objective_func(315, cube);
    swap(cube, i, j, k, 0, 0, a); // Revert back

    swap(cube, i, j, k, 0, a, 0); // Another Swap with different target
    int temp_err2 = objective_func(315, cube);
    swap(cube, i, j, k, 0, a, 0);

    swap(cube, i, j, k, a, 0, 0);
    int temp_err3 = objective_func(315, cube);
    swap(cube, i, j, k, a, 0, 0);

    min_val = std::min(temp_err2, std::min(temp_err3, temp_err1));
    if (min_val == temp_err1) {
      swap(cube, i, j, k, 0, 0, a);
    } else if (min_val == temp_err2) {
      swap(cube, i, j, k, 0, a, 0);
    } else {
      swap(cube, i, j, k, a, 0, 0);
    }
  }
  return min_val;
}

void steepest_ascent_hill_climbing() {
  bool found = false;
  // int temp_cube[N][N][N];
  // copy_cube(temp_cube);
  int currentErr = objective_func(315, cube);
  while (!found) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
          for (int a = 0; a < N; a++) {
            swap(cube, i, j, k, 0, 0, a);
            int temp_err1 = objective_func(315, cube);
            swap(cube, i, j, k, 0, 0, a); // Revert back
          }
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  std::srand(
      static_cast<unsigned int>(std::time(nullptr))); // Seed for randomness

  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Magic Cube 5x5x5");

  // Initialize OpenGL settings and cube data
  init();

  // Register callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouseButton);  // Register mouse button event handler
  glutMotionFunc(mouseMotion); // Register mouse motion event handler
  // std::thread cubeUpdater(testUpdateCube); // Different thread for
  // updating cube

  // Enter the main loop
  glutMainLoop();

  // cubeUpdater.join(); // Wait for the updateCube thread to finish before
  // exiting
  return 0;
}
