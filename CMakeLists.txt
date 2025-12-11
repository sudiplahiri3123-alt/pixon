cmake_minimum_required(VERSION 3.10)
project(pixon)

set(CMAKE_CXX_STANDARD 17)

# Build the core library
add_subdirectory(core)

# Build the CLI (which links to the core library)
add_subdirectory(cli)
