#include "cube.hpp"
#include <random_restart.hpp>
#include <unordered_set>

int random_restart::numOfRestart = 0;

void random_restart::setRestartNum(int n) { random_restart::numOfRestart = n; }

void random_restart::hill_climbing() {

  int count = 0;
  if (std::remove("randomrestart.txt") == 0) {
    std::cout << "randomrestart.txt was deleted successfully.\n";
  } else {
    std::cout << "randomrestart.txt did not exist or couldn't be deleted.\n";
  }

  std::ofstream result("randomrestart.txt", std::ios::app);
  if (!result) {
    std::cerr << "Error opening randomrestart.txt for writing." << std::endl;
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
  // Set restart to 2
  setRestartNum(2);

  int bestCube[cube::N][cube::N][cube::N];
  int bestCubeErr = 9999;
  int noteScore[numOfRestart];

  for (int r = 0; r < numOfRestart; r++) {

    while (true) {
      cube::errInfo neighbor;
      cube::errInfo targetVal;

      int currentErr = cube::objective_func();
      count++;

      if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
      }

      file << currentErr << ";";
      // Loop trough all elements until finish condition satisfied
      for (int i = 0; i < cube::N; i++) {
        for (int j = 0; j < cube::N; j++) {
          for (int k = 0; k < cube::N; k++) {
            // For each elements
            // Count the error after swapping, then find the smallest one
            cube::errInfo temp = cube::swap_cube(i, j, k);
            if (neighbor.error > temp.error) {
              neighbor.error = temp.error;
              neighbor.x = temp.x;
              neighbor.y = temp.y;
              neighbor.z = temp.z;

              // Which Element to be swapped
              targetVal.x = i;
              targetVal.y = j;
              targetVal.z = k;
            }
          }
        }
      }

      if (neighbor.error < currentErr) {
        // std::cout << "Swapped" << std::endl;
        cube::swap(targetVal.x, targetVal.y, targetVal.z, neighbor.x,
                   neighbor.y, neighbor.z);
        // std::cout << "Current Err : " << neighbor.error << std::endl;
      }

      if (currentErr <= neighbor.error) {
        std::cout << "====Hill Climbing Finished ====" << std::endl;
        break;
      }
    }

    file.close();

    // Read the content from swap.txt and prepend a new line in
    // randomrestart.txt
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

    // Find best Cube
    noteScore[r] = cube::objective_func();
    if (bestCubeErr > noteScore[r]) {
      bestCubeErr = noteScore[r];
      cube::copyCube(cube::cube, bestCube);
    }
    cube::restart_cube();
  }

  cube::copyCube(bestCube, cube::cube);

  // for (int r = 0; r < numOfRestart; r++) {
  //   //std::cout << "Score " << r << " :" << noteScore[r] << std::endl;
  // }
  // std::cout << "Now Score :" << cube::objective_func() << std::endl;
  // std::cout << "===== Random Restart Done =====" << std::endl;
}
