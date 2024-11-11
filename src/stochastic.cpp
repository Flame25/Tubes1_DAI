#include "cube.hpp"
#include <cstdlib>
#include <stochastic.hpp>

int stochastic::max_iteration = 5000;
void stochastic::setMaxIter(int n) { max_iteration = n; }

// Randomizer works by taking some 10 random swap and choose best
void stochastic::random_swap() {

  int count = 0;
  // Seed the random number generator with the current time
  cube::errInfo bestData;
  bestData.error = cube::objective_func();
  bool improved = false;
  cube::errInfo target;
  for (int r = 0; r < 100; r++) {
    //  Generate a random number between 0 and 124
    int i_1 = std::rand() % 5; // 125 gives a range of 0-124
    int j_1 = std::rand() % 5;
    int k_1 = std::rand() % 5;

    int i_2 = std::rand() % 5;
    int j_2 = std::rand() % 5;
    int k_2 = std::rand() % 5;

    cube::swap(i_1, j_1, k_1, i_2, j_2, k_2);
    int currentErr = cube::objective_func();
    if (bestData.error > currentErr) {
      bestData.error = currentErr;
      bestData.x = i_1;
      bestData.y = j_1;
      bestData.z = k_1;

      target.x = i_2;
      target.y = j_2;
      target.z = k_2;
      improved = true;
    }
    cube::swap(i_1, j_1, k_1, i_2, j_2, k_2);
  }

  // std::cout << bestData.x << " " << bestData.y << " " << bestData.z << " "
  //          << target.x << " " << target.y << " " << target.z << std::endl;
  if (improved)
    cube::swap(bestData.x, bestData.y, bestData.z, target.x, target.y,
               target.z);
}
void stochastic::hill_climbing() {
  setMaxIter(10000);
  int count = 0;

  if (std::remove("stochastic.txt") == 0) {
    std::cout << "stochastic.txt was deleted successfully.\n";
  } else {
    std::cout << "stochastic.txt did not exist or couldn't be deleted.\n";
  }

  std::ofstream result("stochastic.txt", std::ios::app);
  if (!result) {
    std::cerr << "Error opening stochastic.txt for writing." << std::endl;
    return;
  }

  if (std::remove("swap.txt") == 0) {
    std::cout << "swap.txt was deleted successfully.\n";
  } else {
    std::cout << "swap.txt did not exist or couldn't be deleted.\n";
  }

  // Open swap.txt in append mode for writing scores
  std::ofstream file("swap.txt", std::ios::app);
  if (!file) {
    std::cerr << "Error opening swap.txt for writing." << std::endl;
    return;
  }
  for (int i = 0; i < max_iteration; i++) {
    int currentErr = cube::objective_func();
    random_swap();
    if (!file) {
      std::cerr << "Error opening file." << std::endl;
      return;
    }

    file << currentErr << ";";
  }
  std::cout << "Final Error : " << cube::objective_func() << "\n"
            << "===== Stochastic Hill Climbing Done =====" << std::endl;

  file.close();

  // Read the content from swap.txt and prepend a new line in
  // steepAscent.txt
  std::ifstream read_file("swap.txt");
  if (!read_file) {
    std::cerr << "Error opening swap.txt for reading." << std::endl;
    return;
  }

  std::stringstream buffer;
  buffer << read_file.rdbuf();

  std::cout << "Hello : " << buffer.str() << std::endl;
  read_file.close();

  result << count << std::endl;
  result << buffer.str();
  result.close();
  return;
}
