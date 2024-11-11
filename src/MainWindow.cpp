#include <QtGui>
#include <QtOpenGL/QtOpenGL>
#include <qobjectdefs.h>
#include <qopenglwindow.h>

#include "GLWidget.h"
#include "MainWindow.h"
#include "cube.hpp"
#include "genetic_algorithm.hpp"
#include "simulated_annealing.hpp"
#include "steep_ascent.hpp"
#include "stochastic.hpp"

MainWindow::MainWindow() {
  centralWidget = new QWidget;
  setCentralWidget(centralWidget);

  glWidget = new GLWidget;
  pixmapLabel = new QLabel;

  glWidgetArea = new QScrollArea;
  glWidgetArea->setWidget(glWidget);
  glWidgetArea->setWidgetResizable(true);
  glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  glWidgetArea->setMinimumSize(50, 50);

  pixmapLabelArea = new QScrollArea;
  pixmapLabelArea->setWidget(pixmapLabel);
  pixmapLabelArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  pixmapLabelArea->setMinimumSize(50, 50);

  xSlider =
      createSlider(SIGNAL(xRotationChanged(int)), SLOT(setXRotation(int)));
  ySlider =
      createSlider(SIGNAL(yRotationChanged(int)), SLOT(setYRotation(int)));
  zSlider =
      createSlider(SIGNAL(zRotationChanged(int)), SLOT(setZRotation(int)));

  createActions();
  createMenus();

  QGridLayout *centralLayout = new QGridLayout;
  centralLayout->addWidget(glWidgetArea, 0, 0, 1, 2);
  //    centralLayout->addWidget(pixmapLabelArea, 0, 1);
  centralLayout->addWidget(xSlider, 1, 0, 1, 1);
  centralLayout->addWidget(ySlider, 2, 0, 1, 1);
  centralLayout->addWidget(zSlider, 3, 0, 1, 1);
  centralWidget->setLayout(centralLayout);

  xSlider->setValue(468);
  ySlider->setValue(5064);
  zSlider->setValue(0 * 16);

  setWindowTitle(tr("5x5x5 Cube"));
  resize(700, 700);
}

void MainWindow::renderIntoPixmap() {
  QSize size = getSize();
  if (size.isValid()) {
    QPixmap pixmap = glWidget->grab();
    pixmap = pixmap.scaled(size);
    setPixmap(pixmap);
  }
}

void MainWindow::grabFrameBuffer() {
  QImage image = glWidget->grabFramebuffer();
  setPixmap(QPixmap::fromImage(image));
}

void MainWindow::clearPixmap() { setPixmap(QPixmap()); }

void MainWindow::about() {
  QMessageBox::about(
      this, tr("About Grabber"),
      tr("The <b>Grabber</b> example demonstrates two approaches for "
         "rendering OpenGL into a Qt pixmap."));
}

void MainWindow::runSteepestAsc() {
  steep_ascent::hill_climbing();
  glWidget->redrawGL();
  steep_ascent::drawGraph();
  glWidget->redrawGL();
}

void MainWindow::runRandomRestart() {
  random_restart::hill_climbing();
  glWidget->redrawGL();
  cube::drawGraph("randomrestart");
  glWidget->redrawGL();
}

void MainWindow::runSimulatedAnnealing() {
  simulated_annealing::work_func();
  glWidget->redrawGL();
  cube::drawGraph("simulatedannealing");
  glWidget->redrawGL();
}

void MainWindow::runStochastic() {
  stochastic::hill_climbing();
  glWidget->redrawGL();
  cube::drawGraph("stochastic");
  glWidget->redrawGL();
}

void MainWindow::runSideways() {
  side_ways::hill_climbing();
  glWidget->redrawGL();
  cube::drawGraph("sideways");
  glWidget->redrawGL();
}

void MainWindow::runGenetic() {
  genetic_algorithm::work_func();
  glWidget->redrawGL();
  cube::drawGraph("genetic");
  glWidget->redrawGL();
}

void MainWindow::createActions() {
  renderIntoPixmapAct = new QAction(tr("&Render into Pixmap..."), this);
  renderIntoPixmapAct->setShortcut(tr("Ctrl+R"));
  connect(renderIntoPixmapAct, SIGNAL(triggered()), this,
          SLOT(renderIntoPixmap()));

  grabFrameBufferAct = new QAction(tr("&Grab Frame Buffer"), this);
  grabFrameBufferAct->setShortcut(tr("Ctrl+G"));
  connect(grabFrameBufferAct, SIGNAL(triggered()), this,
          SLOT(grabFrameBuffer()));

  clearPixmapAct = new QAction(tr("&Clear Pixmap"), this);
  clearPixmapAct->setShortcut(tr("Ctrl+L"));
  connect(clearPixmapAct, SIGNAL(triggered()), this, SLOT(clearPixmap()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  runSteepest = new QAction(tr("&Steepest Ascent"), this);
  connect(runSteepest, SIGNAL(triggered()), this, SLOT(runSteepestAsc()));

  runRandom = new QAction(tr("&Random Restart"), this);
  connect(runRandom, SIGNAL(triggered()), this, SLOT(runRandomRestart()));

  runSide = new QAction(tr("&Sideways"), this);
  connect(runSide, SIGNAL(triggered()), this, SLOT(runSideways()));

  runSimulated = new QAction(tr("&Simulated Annealing"), this);
  connect(runSimulated, SIGNAL(triggered()), this,
          SLOT(runSimulatedAnnealing()));

  runGen = new QAction(tr("&Genetic Algorithm"), this);
  connect(runGen, SIGNAL(triggered()), this, SLOT(runGenetic()));

  runSto = new QAction(tr("&Stochastic"), this);
  connect(runSto, SIGNAL(triggered()), this, SLOT(runStochastic()));

  aboutAct = new QAction(tr("&About"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(renderIntoPixmapAct);
  fileMenu->addAction(grabFrameBufferAct);
  fileMenu->addAction(clearPixmapAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  algoMenu = menuBar()->addMenu(tr("&Algorithm"));
  algoMenu->addAction(runSteepest);
  algoMenu->addAction(runSide);
  algoMenu->addAction(runRandom);
  algoMenu->addAction(runSimulated);
  algoMenu->addAction(runSto);
  algoMenu->addAction(runGen);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

QSlider *MainWindow::createSlider(const char *changedSignal,
                                  const char *setterSlot) {
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 360 * 16);
  slider->setSingleStep(16);
  slider->setPageStep(15 * 16);
  slider->setTickInterval(15 * 16);
  slider->setTickPosition(QSlider::TicksRight);
  connect(slider, SIGNAL(valueChanged(int)), glWidget, setterSlot);
  connect(glWidget, changedSignal, slider, SLOT(setValue(int)));
  return slider;
}

void MainWindow::setPixmap(const QPixmap &pixmap) {
  pixmapLabel->setPixmap(pixmap);
  QSize size = pixmap.size();
  if (size - QSize(1, 0) == pixmapLabelArea->maximumViewportSize())
    size -= QSize(1, 0);
  pixmapLabel->resize(size);
}

QSize MainWindow::getSize() {
  bool ok;
  QString text = QInputDialog::getText(
      this, tr("Grabber"), tr("Enter pixmap size:"), QLineEdit::Normal,
      tr("%1 x %2").arg(glWidget->width()).arg(glWidget->height()), &ok);
  if (!ok)
    return QSize();

  QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
  if (regExp.exactMatch(text)) {
    int width = regExp.cap(1).toInt();
    int height = regExp.cap(2).toInt();
    if (width > 0 && width < 2048 && height > 0 && height < 2048)
      return QSize(width, height);
  }

  return glWidget->size();
}
