cmake_minimum_required(VERSION 3.10) # Or higher, C++17 features need 3.10+
project(ChessEngine CXX)

# List all your source files. CMake will figure out dependencies and compile/link them.
set(SOURCES
    src/main.cpp
    src/constants.cpp
    src/board.cpp
    src/display.cpp
    src/move/pawn.cpp
    src/move/bishop.cpp
    src/move/king.cpp
    src/move/move.cpp
    src/move/knight.cpp
    src/move/piece.cpp
    src/move/queen.cpp
    src/move/rook.cpp
    src/move/sliders.cpp
)

# Add an executable target FIRST
add_executable(${PROJECT_NAME} ${SOURCES}) # PROJECT_NAME expands to ChessEngine

# Now, specify the include directory for the *already defined* target
# This tells CMake where to look for header files like "types/types.h"
# Assuming your "src" directory (or a subdirectory within it) contains common headers.
# If "types" is a subdirectory of "src", then src itself needs to be an include directory.
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src") # Assuming headers are in src/ or subfolders of src/

# Add C++ standard (important for features like C++17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Add common compile flags (e.g., warnings)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# Specify output directory (optional, but good practice)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist")