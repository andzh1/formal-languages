cmake_minimum_required(VERSION 3.0)

project(Finite_automatas)

#set(CMAKE_CXX_STANDARD 17)

set(SOURCE_LIB Finite_automatas.cpp)

add_library(Finite_automatas_library SHARED ${SOURCE_LIB})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_executable(Tests tests.cpp)

target_link_libraries(Tests Finite_automatas_library)