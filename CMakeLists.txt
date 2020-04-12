cmake_minimum_required(VERSION 3.0.0)

# project name and version
project(chip8EMU VERSION 0.1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# SDL2 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# GLFW
find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})

# 
IF(APPLE)
    INCLUDE_DIRECTORIES ( /Developer/Headers/FlatCarbon )
    find_library(COCOA_LIBRARY Cocoa)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(OPENGL_LIBRARY OpenGL)
    MARK_AS_ADVANCED (COCOA_LIBRARY IOKIT_LIBRARY COREVIDEO_LIBRARY OPENGL_LIBRARY)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${OPENGL_LIBRARY})
ENDIF (APPLE)

# glad
find_package(GLAD REQUIRED)
include_directories(${GLAD_INCLUDE_DIR})


# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit")

set(PROJECT_HEADERS include/emulator.hpp include/renderer.hpp)
set(PROJECT_SOURCES ${PROJECT_HEADERS} src/emulator.cpp src/renderer.cpp)

add_executable(Chip8Emulator src/main.cpp ${PROJECT_SOURCES})
# SDL2
target_link_libraries(Chip8Emulator ${SDL2_LIBRARIES})
# GLFW
target_link_libraries(Chip8Emulator ${GLFW3_LIBRARY})
target_link_libraries(Chip8Emulator ${EXTRA_LIBS})
# glad
target_link_libraries(Chip8Emulator ${GLAD_LIBRARY})



##################
# Testing
##################
# Add google test
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(tests ${PROJECT_SOURCES} tests/main.cpp)
target_link_libraries (tests gmock_main)
# SDL
target_link_libraries(tests ${SDL2_LIBRARIES})
# GLFW
target_link_libraries(tests ${GLFW3_LIBRARY})
target_link_libraries(tests ${EXTRA_LIBS})
# glad
target_link_libraries(tests ${GLAD_LIBRARY})
add_test(NAME example_test COMMAND example)