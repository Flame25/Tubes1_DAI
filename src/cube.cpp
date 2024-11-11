#include "matplotlibcpp.h"
#include <cctype>
#include <climits>
#include <cstdlib>
#include <cube.hpp>
#include <string>
#include <vector>

int cube::cube[cube::N][cube::N][cube::N]; // Definition and initialization
int cube::isTrue[cube::N][cube::N][cube::N];
bool cube::statusNew = true;

std::vector<int> cube::flatCube(int cube[cube::N][cube::N][cube::N]) {
  std::vector<int> flat(125);
  for (int i = 0; i < cube::N; ++i) {
    for (int j = 0; j < cube::N; ++j) {
      for (int k = 0; k < cube::N; ++k) {
        flat[i * 25 + j * 5 + k] = cube[i][j][k];
      }
    }
  }
  return flat;
}

void cube::unflattenCube(std::vector<int> flat,
                         int cube[cube::N][cube::N][cube::N]) {
  int new_cube[cube::N][cube::N][cube::N];
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 5; ++j)
      for (int k = 0; k < 5; ++k)
        new_cube[i][j][k] = flat[i * 25 + j * 5 + k];
}

// Display per Layer of Cube via CMD
void cube::displayCube() {
  for (int i = 0; i < N; ++i) {
    std::cout << "Layer " << i + 1 << ":\n";
    for (int j = N - 1; j >= 0; --j) {
      for (int k = 0; k < N; ++k) {
        std::cout << cube[i][j][k] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
  // INFO:Somehow cannot display text after loop finished -> Forget to add
  // std::endl

  std::cout << "==== Error ====" << std::endl;
  std::cout << objective_func() << std::endl;
}

// Initialize the cube with unique and random values (1 to N^5)
void cube::initCube(std::unordered_set<int> existingValues) {
  int count = 1;
  std::srand(static_cast<unsigned int>(time(nullptr))); // Seed for randomness
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        int randomValue;
        do {
          randomValue =
              rand() % 125 + 1; // Generates a number between 1 and 125
        } while (existingValues.find(randomValue) !=
                 existingValues.end()); // Ensure uniqueness

        // Add the value to the set of existing values
        existingValues.insert(randomValue);

        // Assign the unique value to the cube at position (i, j, k)
        cube[i][j][k] = randomValue;
        count++;

        isTrue[i][j][k] = 1;
      }
    }
  }
  displayCube();
}

// Helper function to reset sums to zero
void cube::resetSums(std::vector<int> &sums) {
  std::fill(sums.begin(), sums.end(), 0);
}

// Helper function to add absolute differences from 315 to total_sum
int cube::calculate(const std::vector<int> &sums, int target) {
  int total_diff = 0;
  for (int sum : sums) {
    // std::cout << sum << std::endl;
    if (sum != 0)
      total_diff += (sum != 315);
  }
  return total_diff;
}

// Cube Objective Function
int cube::objective_func(int cube[cube::N][cube::N][cube::N]) {
  int total_sum = 0;
  std::vector<int> sums(6, 0); // For storing sum1 to sum6
  const int target_sum = 315;

  // Horizontal, Vertical, Pillar
  for (int i = 0; i < cube::N; i++) {
    for (int j = 0; j < cube::N; j++) {
      for (int k = 0; k < cube::N; k++) {
        sums[0] += cube[i][j][k];
        sums[1] += cube[k][i][j];
        sums[2] += cube[i][k][j];
      }
      total_sum += calculate(sums, target_sum);
      resetSums(sums);
    }
  }

  // Space Diagonal
  for (int i = 0; i < cube::N; i++) {
    sums[0] = cube[i][i][i];
    sums[1] = cube[5 - i][i][i];
    sums[2] = cube[5 - i][i][5 - i];
    sums[3] = cube[i][i][5 - i];
  }
  total_sum += calculate(sums, target_sum);
  resetSums(sums);

  // Plane Diagonal
  for (int i = 0; i < cube::N; i++) {
    for (int j = 0; j < cube::N; j++) {
      sums[0] += cube[i][j][j];
      sums[1] += cube[j][i][j];
      sums[2] += cube[j][j][i];
      sums[3] += cube[5 - j][j][i];
      sums[4] += cube[i][j][5 - j];
      sums[5] += cube[5 - j][i][j];
    }
    total_sum += calculate(sums, target_sum);
    resetSums(sums);
  }

  return total_sum;
}

// Cube Objective Function
int cube::objective_func() {
  int total_sum = 0;
  statusNew = false;
  std::vector<int> sums(6, 0); // For storing sum1 to sum6
  const int target_sum = 315;

  // Horizontal, Vertical, Pillar
  for (int i = 0; i < cube::N; i++) {
    for (int j = 0; j < cube::N; j++) {
      for (int k = 0; k < cube::N; k++) {
        sums[0] += cube[i][j][k];
        sums[1] += cube[k][i][j];
        sums[2] += cube[i][k][j];
      }
      total_sum += calculate(sums, target_sum);

      for (int k = 0; k < cube::N; k++) {
        isTrue[i][j][k] &= sums[0] == 315;
        isTrue[k][i][j] &= sums[1] == 315;
        isTrue[i][k][j] &= sums[2] == 315;
      }

      resetSums(sums);
    }
  }

  // Space Diagonal
  for (int i = 0; i < cube::N; i++) {
    sums[0] = cube[i][i][i];
    sums[1] = cube[5 - i][i][i];
    sums[2] = cube[5 - i][i][5 - i];
    sums[3] = cube[i][i][5 - i];
  }
  total_sum += calculate(sums, target_sum);
  for (int i = 0; i < cube::N; i++) {
    isTrue[i][i][i] &= sums[0] == 315;
    isTrue[5 - i][i][i] &= sums[1] == 315;
    isTrue[5 - i][i][5 - i] &= sums[2] == 315;
    isTrue[i][i][5 - i] &= sums[3] == 315;
  }
  resetSums(sums);

  // Plane Diagonal
  for (int i = 0; i < cube::N; i++) {
    for (int j = 0; j < cube::N; j++) {
      sums[0] += cube[i][j][j];
      sums[1] += cube[j][i][j];
      sums[2] += cube[j][j][i];
      sums[3] += cube[5 - j][j][i];
      sums[4] += cube[i][j][5 - j];
      sums[5] += cube[5 - j][i][j];
    }
    total_sum += calculate(sums, target_sum);

    for (int j = 0; j < cube::N; j++) {
      isTrue[i][j][j] &= sums[0] == 315;
      isTrue[j][i][j] &= sums[1] == 315;
      isTrue[j][j][i] &= sums[2] == 315;
      isTrue[5 - j][j][i] &= sums[3] == 315;
      isTrue[i][j][5 - j] &= sums[4] == 315;
      isTrue[5 - j][i][j] &= sums[5] == 315;
    }
    resetSums(sums);
  }

  return total_sum;
}

void cube::swap(int x1, int y1, int z1, int x2, int y2, int z2) {
  // std::cout << x1 << " " << y1 << " " << z1 << " " << x2 << " " << y2 << " "
  //<< z2 << std::endl;
  int temp = cube::cube[x1][y1][z1];
  cube::cube[x1][y1][z1] = cube::cube[x2][y2][z2];
  cube::cube[x2][y2][z2] = temp;
}

// Implement by check all the possible movement first
// Then, check the lowest error and change cube to that position
cube::errInfo cube::swap_cube(int x, int y, int z) {
  std::vector<cube::errInfo> error_list;

  cube::errInfo noted_error;

  // Try All Possible Places
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {

        swap(x, y, z, i, j, k);
        int test_error = objective_func();
        if (noted_error.error > test_error) {
          noted_error.error = test_error;
          noted_error.x = i;
          noted_error.y = j;
          noted_error.z = k;
        }
        swap(x, y, z, i, j, k);
      }
    }
  }
  return noted_error;
}

// Copy one cube to another
void cube::copyCube(int (*first)[5][5], int (*target)[5][5]) {

  std::cout << "Copying Cube" << std::endl;
  for (int i = 0; i < cube::N; i++) {
    for (int j = 0; j < cube::N; j++) {
      for (int k = 0; k < cube::N; k++) {
        target[i][j][k] = first[i][j][k];
      }
    }
  }
}

void cube::restart_cube() {
  std::unordered_set<int> existingValues;
  cube::initCube(existingValues);
}

void cube::drawGraph(std::string name) {
  std::string algoName = name;
  transform(algoName.begin(), algoName.end(), algoName.begin(), ::tolower);
  std::string filename = algoName + ".txt";
  std::cout << algoName << std::endl;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  int length;
  std::vector<int> data;

  // Read the first line for the length
  std::string line;
  if (std::getline(file, line)) {
    length = std::stoi(line); // Convert the first line to an integer for length
  }

  // Read the next line(s) for the data
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string value;

    // Split by ';' and parse each value as integer
    while (std::getline(ss, value, ';')) {
      if (!value.empty()) {
        data.push_back(std::stoi(value));
      }
    }
  }

  file.close();

  // Verify if the number of data entries matches the length specified
  if (data.size() != length) {
    std::cerr << "Warning: Data size (" << data.size()
              << ") does not match length (" << length << ")" << std::endl;
  }

  // Print the results
  std::cout << "Length: " << length << std::endl;
  std::cout << "Data: ";
  for (int num : data) {
    std::cout << num << " ";
  }
  matplotlibcpp::plot(data);
  matplotlibcpp::show();
}
