#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include <QtGlobal>
#include <math.h>
#include <sstream>

#ifdef Q_OS_WIN
#include "gl/GLU.h"
#else

#include <GL/glut.h>

#endif

#include "GLWidget.h"
#include "cube.hpp"
#include <QDateTime>
#include <QDebug>
#include <iostream>

#define PI 3.1459

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << Q_FUNC_INFO;
  if (false) {
    xRot = 468;
    yRot = 5064;
    zRot = 0;
    m_panX = -10;
    m_panY = 6.375;
    m_time = 0;
    timeBasedVar = 0;
    zoomFactor = -70;
  } else {
    xRot = 0;
    yRot = 0;
    zRot = 0;
    m_panX = 0;
    m_panY = 0;
    m_time = 0;
    timeBasedVar = 0;
    zoomFactor = -70;
  }
  int countPerSide = 5;

  for (int x = 0; x < countPerSide; x++) {
    spheresInSpace.append(QVector<QVector<MySphere *>>());
    for (int y = 0; y < countPerSide; y++) {
      spheresInSpace[x].append(QVector<MySphere *>());
      for (int z = 0; z < countPerSide; z++) {
        MySphere *sph = new MySphere();
        sph->center = QVertex(x, y, z);
        sph->radius = 0.3;
        allSpheres.append(sph);
        spheresInSpace[x][y].append(sph);

        // TODO: Add random number 1 - 25
      }
    }
  }

  int mid = countPerSide / 2;
  center = spheresInSpace[mid][mid][mid]->center;

  //    grabKeyboard();

  //    QTimer *timer = new QTimer(this);
  //    connect(timer, SIGNAL(timeout()), this, SLOT(advanceGears()));
  //    timer->start(20);
}

GLWidget::~GLWidget() {
  makeCurrent();

  foreach (MySphere *s, allSpheres) {
    glDeleteLists(s->glQuadric, 1);
  }

  qDeleteAll(allSpheres);
  allSpheres.clear();
}

void GLWidget::setXRotation(int angle) {
  normalizeAngle(&angle);
  if (angle != xRot) {
    xRot = angle;
    emit xRotationChanged(angle);
    update();
  }
}

void GLWidget::setYRotation(int angle) {
  normalizeAngle(&angle);
  if (angle != yRot) {
    yRot = angle;
    emit yRotationChanged(angle);
    update();
  }
}

void GLWidget::setZRotation(int angle) {
  normalizeAngle(&angle);
  if (angle != zRot) {
    zRot = angle;
    emit zRotationChanged(angle);
    update();
  }
}

void GLWidget::setReflectance(GLfloat *glcolor, QColor color) {
  glcolor[0] = color.redF();
  glcolor[2] = color.blueF();
  glcolor[1] = color.greenF();
  glcolor[3] = color.alphaF();
}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  // Initialize GLUT if it's not already done
  int argc = 1; // We need to pass some argument count (even if it's a dummy)
  char *argv[1] = {(char *)"MyQtApp"};
  glutInit(&argc, argv); // Call glutInit to initialize GLUT
  // Check OpenGL version
  qDebug() << "OpenGL Version:" << glGetString(GL_VERSION);
  qDebug() << "OpenGL Renderer:" << glGetString(GL_RENDERER);
  qDebug() << "OpenGL Vendor:" << glGetString(GL_VENDOR);
  qDebug() << Q_FUNC_INFO;
  static const GLfloat lightPos[4] = {5.0f, 5.0f, 10.0f, 1.0f};

  //    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  //    glEnable(GL_LIGHTING);
  //    glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  // Enable blending for transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Restore to default polygon mode after rendering wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Fill mode for regular
  // rendering

  // Disable blending (or reset if needed for other parts of your scene)
  //    glDisable(GL_BLEND);
  //
  //    glEnable(GL_NORMALIZE);
  //    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  std::unordered_set<int> existingValues;
  cube::initCube(existingValues);
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glPushMatrix();

  // Set up the camera (view) using gluLookAt
  gluLookAt(0.0, 0.0, 10.0, // Camera position (x, y, z)
            0.0, 0.0, 0.0,  // Look-at point (x, y, z)
            0.0, 1.0, 0.0); // Up direction (x, y, z)

  glTranslated(m_panX, m_panY, zoomFactor);
  glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
  glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
  glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

  // Apply transformations (rotate the whole scene if necessary)
  glTranslatef(0.0f, 0.0f, -5.0f); // Move the cube back along the z-axis

  // Rotate the scene (if you want to animate or rotate the Rubik's cube)
  glRotatef(3, 1.0f, 1.0f, 0.0f); // Rotate around the x-axis and y-axis
  drawAllCube();                  // Draw the Rubik's Cube or the grid of cubes

  glPopMatrix();
}

void GLWidget::resizeGL(int width, int height) {
  qDebug() << Q_FUNC_INFO;
  if (false) {
    // make the viewport a fitted square
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
  } else {
    glViewport(0, 0, width, height);
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // left, right, top bottom near far
  glFrustum(-1.0 * (qreal)width / (qreal)height,
            +1.0 * (qreal)width / (qreal)height, -1.0, 1.0, 5.0, 200.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -40.0);
}

void GLWidget::wheelEvent(QWheelEvent *e) {
  zoomFactor += e->delta() / 15 / 8;

  update();
}

void GLWidget::mousePressEvent(QMouseEvent *event) { lastPos = event->pos(); }

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  qreal dx = event->x() - lastPos.x();
  qreal dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::MiddleButton) {
    setXRotation(xRot + 8 * dy);
    setYRotation(yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(xRot + 8 * dy);
    setZRotation(zRot + 8 * dx);
  } else if (event->buttons() & Qt::LeftButton) {
    m_panX += dx / 16;
    m_panY -= dy / 16;
  }
  lastPos = event->pos();
  update();
}

void GLWidget::advanceGears() {
  timeBasedVar += 2 * 16;
  update();
}

GLuint GLWidget::makeSphere(MySphere s, const std::string number) {
  GLuint list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, s.reflectance);

  glShadeModel(GL_FLAT);
  GLUquadric *sphere = gluNewQuadric();

  int slices = 15;
  int stacks = slices * 2 / 3;

  glTranslated(s.center.x(), s.center.y(), s.center.z());
  gluSphere(sphere, s.radius, slices, stacks);

  gluDeleteQuadric(sphere); // Always delete the quadric when done
  glEndList();

  return list;
}

void GLWidget::renderText(float x, float y, float z, const std::string &text) {
  glColor3f(1.0f, 1.0f,
            1.0f);        // Set the text color to white (RGB: 1.0f, 1.0f, 1.0f)
  glRasterPos3f(x, y, z); // Set the position for text
  glScalef(5.0f, 5.0f, 5.0f); // Scale the text (increase size)
  for (char c : text) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Draw each character
  }
}

void GLWidget::drawSphere(MySphere s) {
  glPushMatrix();
  glTranslated(s.center.x(), s.center.y(), s.center.z());
  glCallList(s.glQuadric);
  glPopMatrix();
}

void GLWidget::drawPendulum(Pendulum p, GLdouble angle) {
  glPushMatrix();
  glTranslated(p.pt.x(), p.pt.y(), p.pt.z());
  glRotated(angle, 0.0, 0.0, 1.0);
  glCallList(p.glQuadric);
  glPopMatrix();
}

void GLWidget::drawCube(float x, float y, float z, int status) {
  glPushMatrix();
  glTranslatef(x, y, z);

  glBegin(GL_QUADS);
  // Define a color once
  GLfloat redColor[] = {1.0f, 0.0f, 0.0f, 0.13f};   // Semi-transparent red
  GLfloat greenColor[] = {0.0f, 1.0f, 0.0f, 0.13f}; // Semi-transparent green
  GLfloat whiteColor[] = {1.0f, 1.0f, 1.0f, 0.13f}; // Semi-transparent white

  if (cube::statusNew) {
    std::cout << "Hello" << std::endl;
    glColor4fv(whiteColor); // Apply red color to all faces
  } else if (status) {
    // if cube is true or good enough
    glColor4fv(greenColor);
  } else {
    glColor4fv(redColor);
  }

  // Front face
  glVertex3f(-0.25f, -0.25f, 0.25f);
  glVertex3f(0.25f, -0.25f, 0.25f);
  glVertex3f(0.25f, 0.25f, 0.25f);
  glVertex3f(-0.25f, 0.25f, 0.25f);

  // Back face
  glVertex3f(-0.25f, -0.25f, -0.25f);
  glVertex3f(-0.25f, 0.25f, -0.25f);
  glVertex3f(0.25f, 0.25f, -0.25f);
  glVertex3f(0.25f, -0.25f, -0.25f);

  // Left face
  glVertex3f(-0.25f, -0.25f, -0.25f);
  glVertex3f(-0.25f, -0.25f, 0.25f);
  glVertex3f(-0.25f, 0.25f, 0.25f);
  glVertex3f(-0.25f, 0.25f, -0.25f);

  // Right face
  glVertex3f(0.25f, -0.25f, -0.25f);
  glVertex3f(0.25f, 0.25f, -0.25f);
  glVertex3f(0.25f, 0.25f, 0.25f);
  glVertex3f(0.25f, -0.25f, 0.25f);

  // Top face
  glVertex3f(-0.25f, 0.25f, -0.25f);
  glVertex3f(-0.25f, 0.25f, 0.25f);
  glVertex3f(0.25f, 0.25f, 0.25f);
  glVertex3f(0.25f, 0.25f, -0.25f);

  // Bottom face
  glVertex3f(-0.25f, -0.25f, -0.25f);
  glVertex3f(0.25f, -0.25f, -0.25f);
  glVertex3f(0.25f, -0.25f, 0.25f);
  glVertex3f(-0.25f, -0.25f, 0.25f);

  glEnd(); // End drawing

  glPopMatrix();
}

void GLWidget::drawAllCube() {
  std::cout << "Draw Called" << std::endl;
  float spacing = 1.3f; // Spacing between small cubes
  glScalef(3.0f, 3.0f, 3.0f);
  // Loop through a 5x5x5 grid of cubes
  for (int i = 0; i < cube::N; ++i) {
    for (int j = 0; j < cube::N; ++j) {
      for (int k = 0; k < cube::N; ++k) {
        glPushMatrix();
        drawCube(i * spacing, j * spacing, k * spacing,
                 cube::isTrue[i][j][k]); // Draw each small cube at the current
                                         // position

        // Convert the number to string
        std::stringstream ss;
        ss << cube::cube[i][j][k];
        std::string numStr = ss.str();

        glDisable(GL_DEPTH_TEST); // Disable depth testing before rendering text
        renderText(i * spacing - 0.05, j * spacing - 0.1, k * spacing + 0.1,
                   numStr);
        glEnable(GL_DEPTH_TEST); // Re-enable depth testing afterward
        glPopMatrix();           // Restore the matrix state
      }
    }
  }
}

void GLWidget::normalizeAngle(int *angle) {
  while (*angle < 0)
    *angle += 360 * 16;
  while (*angle > 360 * 16)
    *angle -= 360 * 16;
}

void GLWidget::redrawGL() { update(); }
