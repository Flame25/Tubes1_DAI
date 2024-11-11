//
// Created by gadzz on 11/9/24.
//

#include "steep_ascent.hpp"
#include <MainWindow.h>
#include <QApplication>

void runAlgorithm() { steep_ascent::hill_climbing(); }

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
