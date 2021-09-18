# CMake version
cmake_minimum_required(VERSION 2.8)

# Project name & language
project(MyString CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -fPIC -Wall")

# Generating a static library.
# add_library(MyString STATIC MyString.cpp)
add_library(MyAll STATIC MyString.cpp)

# Generating a shared library.
add_library(MyString SHARED MyString.cpp)

# Generating the sample.
add_executable(MyString.sample.exe MyString.sample.cpp)

#target_compile_options(MyString.sample.exe PUBLIC -O2 -Wall)
#target_compile_features(MyString.sample.exe PUBLIC cxx_std_11)

# link the static library with the sample.
target_link_libraries(MyString.sample.exe MyString)
