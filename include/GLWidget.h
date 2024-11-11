#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QColor>
#include <QKeyEvent>
#include <QList>
#include <QOpenGLFunctions> // Provides OpenGL types and functions
#include <QOpenGLWidget>
#include <QVector3D>
#include <QVector>

typedef QVector3D QVertex;

struct Pendulum {
  GLuint glQuadric;
  qreal phase_offset; // added to the time variable
  qreal radius;
  qreal cord_length;
  qreal theta0;
  bool hovered;
  QVertex pt;
  GLfloat reflectance[4];
};

struct MySphere {
  QVertex center;
  GLfloat reflectance[4];
  GLuint glQuadric;
  qreal radius;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  int xRotation() const { return xRot; }
  int yRotation() const { return yRot; }
  int zRotation() const { return zRot; }
  void redrawGL();

public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);

signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *);
  void keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Space)
      m_time = -.2;
  }

private slots:
  void advanceGears();

private:
  QVertex center;
  GLuint makePendulum(Pendulum p);
  //    void drawPendulum(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
  //                  GLdouble angle);

  GLuint makeSphere(MySphere s, const std::string number);
  void drawSphere(MySphere s);
  void renderText(float x, float y, float z, const std::string &text);
  void drawPendulum(Pendulum p, GLdouble angle);
  void normalizeAngle(int *angle);
  void drawCube(float x, float y, float z, int status);
  void drawAllCube();
  void setReflectance(GLfloat *glcolor, QColor color);
  void renderTextOnFace(float x, float y, float z, const std::string &text);

  QList<Pendulum *> pen;
  QVector<QVector<QVector<MySphere *>>> spheresInSpace;
  QList<MySphere *> allSpheres;
  int xRot;
  int yRot;
  int zRot;
  int timeBasedVar;
  int zoomFactor;
  qreal m_panX;
  qreal m_panY;
  qreal m_time;

  QPoint lastPos;
};

#endif
