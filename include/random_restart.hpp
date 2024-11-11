#ifndef random_restart_hpp
#define random_restart_hpp
#include <cube.hpp>

namespace random_restart {
extern int numOfRestart;
void hill_climbing();
void setRestartNum(int n);
// Drawing graphics based on last run algorithm
void drawGraph();
} // namespace random_restart

#endif
