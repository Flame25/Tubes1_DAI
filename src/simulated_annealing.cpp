#include "cube.hpp"
#include <cmath>
#include <simulated_annealing.hpp>

double simulated_annealing::alpha = 0.9;
void setAlpha(int n) { simulated_annealing::alpha = n; }
void simulated_annealing::work_func() {
  double T = 9999.0;

  int count = 0;

  if (std::remove("simulatedannealing.txt") == 0) {
    std::cout << "simulatedannealing.txt was deleted successfully.\n";
  } else {
    std::cout
        << "simulatedannealing.txt did not exist or couldn't be deleted.\n";
  }

  std::ofstream result("simulatedannealing.txt", std::ios::app);
  if (!result) {
    std::cerr << "Error opening simulatedannealing.txt for writing."
              << std::endl;
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
  while (true) {

    int currentErr = cube::objective_func();
    count++;
    if (T <= 0.00000001) {
      break;
    }

    if (!file) {
      std::cerr << "Error opening file." << std::endl;
      return;
    }

    file << currentErr << ";";

    cube::errInfo bestData;
    bool improved = false;
    cube::errInfo target;
    // Choose best candidate between all candidates
    for (int r = 0; r < 1000; r++) {
      //  Generate a random number between 0 and 124
      int i_1 = std::rand() % 5; // 125 gives a range of 0-124
      int j_1 = std::rand() % 5;
      int k_1 = std::rand() % 5;

      int i_2 = std::rand() % 5;
      int j_2 = std::rand() % 5;
      int k_2 = std::rand() % 5;

      cube::swap(i_1, j_1, k_1, i_2, j_2, k_2);
      int swapErr = cube::objective_func();
      if (bestData.error > swapErr) {
        bestData.error = swapErr;
        bestData.x = i_1;
        bestData.y = j_1;
        bestData.z = k_1;

        target.x = i_2;
        target.y = j_2;
        target.z = k_2;
      }
      cube::swap(i_1, j_1, k_1, i_2, j_2, k_2);
    }
    // Count Probs for choosing bad
    double ap = exp((bestData.error - currentErr) / T);
    T *= alpha;

    // std::cout << bestData.x << " " << bestData.y << " " << bestData.z << " "
    //          << target.x << " " << target.y << " " << target.z << std::endl;

    // std::cout << "Selected : " << currentErr << "/" << bestData.error
    //          << std::endl;
    if (bestData.error < currentErr) {
      cube::swap(bestData.x, bestData.y, bestData.z, target.x, target.y,
                 target.z);
    } else if (ap > (double)rand() / RAND_MAX) {
      // std::cout << "Bad Selected : " << currentErr << "/" << bestData.error
      //<< std::endl;
      cube::swap(bestData.x, bestData.y, bestData.z, target.x, target.y,
                 target.z);
    }
  }
  std::cout << " ==== Simulated Annealing Finished ===== " << "\n"
            << "Final Error : " << cube::objective_func() << std::endl;

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
