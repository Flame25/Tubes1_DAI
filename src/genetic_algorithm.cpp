#include "cube.hpp"
#include <algorithm>
#include <cstdlib>
#include <genetic_algorithm.hpp>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <vector>

int population_num = 10;
int N = 5;
double MUTATION_PROB = 0.20;

// FUNCTION FOR CROSSOVER
void genetic_algorithm::crossover(int parent1[cube::N][cube::N][cube::N],
                                  int parent2[cube::N][cube::N][cube::N],
                                  int cube_target1[cube::N][cube::N][cube::N],
                                  int allowedValues[], int allowedSize) {
  std::vector<int> flatcube1;
  std::vector<int> flatcube2;
  std::vector<int> child;

  flatcube1 = cube::flatCube(parent1);
  flatcube2 = cube::flatCube(parent2);
  child = flatcube1;

  int size = flatcube1.size();
  int start = rand() % size;
  int end = rand() % size;

  if (start > end) {
    int temp = end;
    end = start;
    start = temp;
  }

  for (int i = start; i < end; ++i) {
    int parent2_value = flatcube2[i];
    int child_value = child[i];

    if (parent2_value != child_value) {
      auto it = std::find(child.begin(), child.end(), parent2_value);
      if (it != child.end()) {
        int index = std::distance(child.begin(), it);
        child[index] = child_value;
      }
      child[i] = parent2_value;
    }
  }
  int cube1[cube::N][cube::N][cube::N];
  // std::cout<<"unflattencube"<<std::endl;
  cube::unflattenCube(child, cube1);
  applyMutation(cube1, allowedValues, allowedSize);
  // std::cout<<"copying cube"<<std::endl;
  cube::copyCube(cube1, cube_target1);
}

// Function to perform Cycle Crossover on a 2D array (one layer of the 3D array)
void genetic_algorithm::cycleCrossover2D(int parent1Layer[cube::N][cube::N],
                                         int parent2Layer[cube::N][cube::N],
                                         int offspringLayer[cube::N][cube::N]) {
  bool visited[cube::N][cube::N] = {false}; // To track visited elements

  // Cycle Crossover for 2D array
  for (int start = 0; start < N * N; ++start) {
    // std::cout<<"Masuk loop"<<std::endl;
    int row = start / N;
    int col = start % N;

    if (!visited[row][col]) {
      int current = start;
      int cycleLimit = N * N;
      int cycleCount = 0;
      do {

        row = current / N;
        col = current % N;
        // std::cout<<"test"<<row<<col<<std::endl; //DEBUG
        // Copy element from parent1 to offspring
        offspringLayer[row][col] = parent1Layer[row][col];
        visited[row][col] = true;

        // Find the next element in the cycle (from parent2 to parent1)
        int nextElem = parent2Layer[row][col];
        // std::cout<<"masuk"<<std::endl;
        // Search for the element in parent1
        bool found = false;
        for (int r = 0; r < N && !found; ++r) {
          for (int c = 0; c < N && !found; ++c) {
            if (parent1Layer[r][c] == nextElem) {
              // std::cout<<r<<c<<N<<std::endl;
              current = r * N + c;
              found = true;
              // std::cout<<"selesailoop"<<std::endl;
            }
          }
        }

      } while (current != start); // Cycle ends when we reach the start index
    }
  }
  std::cout << "finish2D" << std::endl;
}

// Function to apply mutation based on a probability
void genetic_algorithm::applyMutation(int offspring[cube::N][cube::N][cube::N],
                                      int allowedValues[], int allowedSize) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        // Randomly mutate based on mutation probability
        if ((double)rand() / RAND_MAX < MUTATION_PROB) {
          // Select a random element from allowedValues (ensure no duplicates)
          int newVal;
          bool duplicate;
          do {
            newVal = allowedValues[rand() % allowedSize];
            duplicate = false;
            // Check for duplicates in the current layer
            for (int l = 0; l < N; ++l) {
              if (offspring[i][j][l] == newVal) {
                duplicate = true;
                break;
              }
            }
          } while (duplicate);

          offspring[i][j][k] = newVal; // Mutate the element
        }
      }
    }
  }
}

// Cycle Crossover for 3D arrays with mutation
void genetic_algorithm::cycleCrossoverWithMutation(
    int parent1[cube::N][cube::N][cube::N],
    int parent2[cube::N][cube::N][cube::N],
    int offspring[cube::N][cube::N][cube::N], int allowedValues[],
    int allowedSize) {

  // Apply cycle crossover layer by layer (for each 2D array in the 3D cube)
  for (int i = 0; i < N; ++i) {
    // std::cout<<"masuk"<<i<<std::endl;
    cycleCrossover2D(parent1[i], parent2[i], offspring[i]);
  }

  // Apply mutation after crossover
  applyMutation(offspring, allowedValues, allowedSize);
}

void genetic_algorithm::work_func() {

  if (std::remove("genetic.txt") == 0) {
    std::cout << "genetic.txt was deleted successfully.\n";
  } else {
    std::cout << "genetic.txt did not exist or couldn't be deleted.\n";
  }

  std::ofstream result("genetic.txt", std::ios::app);
  if (!result) {
    std::cerr << "Error opening genetic.txt for writing." << std::endl;
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
  std::set<int> new_population;
  // int population_num = 10;

  std::vector<genetic_algorithm::individual> population;

  // Current generation
  int generation = 0;

  // Create randomized initial cube
  for (int i = 0; i < population_num; i++) {
    individual new_cube;
    new_cube.fitness =
        110 - cube::objective_func(); // Lower objective function value mean
                                      // better fitness so we inverse
    cube::copyCube(cube::cube, new_cube.cube);
    population.push_back(new_cube);
    cube::restart_cube();
  }

  // Start main looping
  // Do roullete n times, cross over
  for (int i = 0; i < 1000; i++) {
    // Sort by fitness in ascending order
    std::sort(population.begin(), population.end(),
              [](const genetic_algorithm::individual &a,
                 const genetic_algorithm::individual &b) {
                return a.fitness > b.fitness;
              });
    double probabilities[population_num]; // for relative probability of each
                                          // individual
    // int all_score[population_num];

    double sum_score = 0;
    for (int i = 0; i < population_num; i++) {
      sum_score += population[i].fitness;
    }

    for (int i = 0; i < population_num; i++) {
      probabilities[i] = population[i].fitness / (sum_score);
    }

    double cummulative_prob = 0;
    for (int i = 0; i < population_num; i++) {
      cummulative_prob += probabilities[i];
      population[i].probabilities = cummulative_prob;
    }

    if (population[0].fitness <= 0) {
      break;
    }

    int allowedValues[N * N * N];
    for (int i = 0; i < N * N * N; ++i) {
      allowedValues[i] = i + 1;
    }
    int allowedSize = N * N * N;

    int firstparent = 0;
    int secondparent = 0;
    bool foundfirst;
    bool foundsecond;

    int max_obj = -99999;
    for (int i = 0; i < population_num / 2; i++) {
      foundfirst = false;
      foundsecond = false;
      double r = ((double)rand() / (RAND_MAX)); // rand value (0,1)
      for (int j = 0; j < population_num; j++) {
        if (population[j].probabilities <= r &&
            !foundfirst) { // if found lowerbound
          firstparent = j;
          foundfirst = true;
          std::cout << "foundfirst" << j << std::endl; // DEBUG
        }
      }

      for (int j = 0; j < population_num; j++) {
        if (population[j].probabilities >= r &&
            !foundsecond) { // if found upperbound
          secondparent = j;
          std::cout << "foundsecond" << j << std::endl; // DEBUG
          foundsecond = true;
        }
      }

      // Offspring array
      int offspring1[cube::N][cube::N][cube::N] = {{{0}}}; // Initialize with 0s
      // Perform cycle crossover with mutation
      // cycleCrossoverWithMutation(population[firstparent].cube,
      //                            population[secondparent].cube,
      //                            offspring1, allowedValues, allowedSize);

      firstparent = 0;
      secondparent = 0;
      crossover(population[firstparent].cube, population[secondparent].cube,
                offspring1, allowedValues, allowedSize);
      std::cout << "crossover finish" << std::endl; // DEBUG
      int num1 = cube::objective_func(offspring1);  // first cube
      max_obj = std::max(num1, max_obj);

      individual new_one1;
      cube::copyCube(offspring1, new_one1.cube);
      new_one1.fitness = 110 - num1; // relative fitness
      population.push_back(new_one1);

      // int num2 = cube::objective_func(offspring2); //second cube
      // individual new_one2;
      // new_one2.fitness = 110 - num2; //relative fitness
      // cube::copyCube(offspring2, new_one2.cube);
      // population.push_back(new_one2);
    }
    if (!file) {
      std::cerr << "Error opening file." << std::endl;
      return;
    }
    file << max_obj << ";";

    std::cout << "Generation: " << generation << "\t" << std::endl;

    generation++;
    // Update population size
  }

  individual smallestOne;
  smallestOne.fitness = -9999;
  // std::cout << "Current Err :" << cube::objective_func() << std::endl;
  for (individual i : population) {
    if (i.fitness < smallestOne.fitness) {
      smallestOne = i;
    }
  }

  file.close();

  // Read the content from swap.txt and prepend a new line in
  // sideways.txt
  std::ifstream read_file("swap.txt");
  if (!read_file) {
    std::cerr << "Error opening swap.txt for reading." << std::endl;
    return;
  }

  std::stringstream buffer;
  buffer << read_file.rdbuf();

  std::cout << "Hello : " << buffer.str() << std::endl;
  read_file.close();

  result << 1000 << std::endl;
  result << buffer.str();
  result.close();
  cube::copyCube(cube::cube, smallestOne.cube);
  cube::displayCube();
  return;

  // std::cout << "New Err :" << smallestOne.fitness << std::endl;
}
