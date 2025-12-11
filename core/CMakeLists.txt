cmake_minimum_required(VERSION 3.10)
project(pixon_core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

set(SOURCES
    src/pixon_core.cpp
    src/pixon_libvips.cpp
    src/pixon_operations.cpp
)

# Build shared library
add_library(pixon_core SHARED ${SOURCES})
target_link_libraries(pixon_core ${OpenCV_LIBS})

# Install library + headers
install(TARGETS pixon_core
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)
