cmake_minimum_required(VERSION 3.0)

project(Finite_automations)

#set(CMAKE_CXX_STANDARD 17)

set(SOURCE_LIB Finite_automations.cpp)

add_library(Finite_automations_library SHARED ${SOURCE_LIB})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_executable(Tests tests.cpp)

target_link_libraries(Tests Finite_automations_library)