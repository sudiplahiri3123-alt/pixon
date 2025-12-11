cmake_minimum_required(VERSION 3.10)
project(pixon_cli)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/include
)

add_executable(pixon main.cpp)

target_link_libraries(pixon pixon_core)

install(TARGETS pixon
        RUNTIME DESTINATION bin)
