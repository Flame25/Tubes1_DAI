#ifndef stochastic_hpp
#define stochastic_hpp
#include <cube.hpp>

namespace stochastic {
extern int max_iteration;
void setMaxIter(int n);
void hill_climbing();

// Randomizer works by taking some 10 random swap and choose best
void random_swap();
} // namespace stochastic

#endif
