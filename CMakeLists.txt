project(MyQtApp)

include(GNUInstallDirs)

# Find and set up Qt libraries (adjust to your Qt version and required components)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Library target for matplotlibcpp
add_library(matplotlib_cpp INTERFACE)

# Find Python3
find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)

# Ensure Python and NumPy are linked correctly
target_link_libraries(matplotlib_cpp INTERFACE
  Python3::Python
  Python3::Module
)

# Include directories for Python3 and NumPy
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})  # Add NumPy's include directories

# Include directories for your own headers
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})

# Set the CMake export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)


# Source and header files
set(SRC_FILES
        src/GLWidget.cpp
        src/main.cpp
        src/MainWindow.cpp
        src/cube.cpp
        src/genetic_algorithm.cpp
        src/steep_ascent.cpp
        src/stochastic.cpp
        src/side_ways.cpp
        src/simulated_annealing.cpp
        src/random_restart.cpp
)

set(HDR_FILES
       include/GLWidget.h
       include/MainWindow.h
)

# Link libraries for Python
link_directories(${Python3_LIBRARY_DIRS})

# Enable Qt moc files generation (important for Qt signals/slots)
set(CMAKE_AUTOMOC ON)

# Define the executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})

# Link Qt libraries and Python libraries
target_link_libraries(${PROJECT_NAME}
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  OpenGL::GL
  GLUT::GLUT
  GLU
  ${Python3_LIBRARIES}
)
