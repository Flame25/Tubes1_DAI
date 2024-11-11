#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;

class QLabel;

class QMenu;

class QScrollArea;

class QSlider;

QT_END_NAMESPACE
class GLWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

private slots:

  void renderIntoPixmap();

  void grabFrameBuffer();

  void clearPixmap();

  void about();

  void runSteepestAsc();

  void runRandomRestart();

  void runSideways();

  void runSimulatedAnnealing();

  void runStochastic();

  void runGenetic();

private:
  void createActions();

  void createMenus();

  QSlider *createSlider(const char *changedSignal, const char *setterSlot);

  void setPixmap(const QPixmap &pixmap);

  QSize getSize();

  QWidget *centralWidget;
  QScrollArea *glWidgetArea;
  QScrollArea *pixmapLabelArea;
  GLWidget *glWidget;
  QLabel *pixmapLabel;
  QSlider *xSlider;
  QSlider *ySlider;
  QSlider *zSlider;

  QMenu *fileMenu;
  QMenu *helpMenu;
  QMenu *algoMenu;
  QAction *grabFrameBufferAct;
  QAction *renderIntoPixmapAct;
  QAction *clearPixmapAct;
  QAction *exitAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *runSteepest;
  QAction *runRandom;
  QAction *runSide;
  QAction *runSimulated;
  QAction *runSto;
  QAction *runGen;
};

#endif
