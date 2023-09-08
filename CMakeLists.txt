# Root

cmake_minimum_required(VERSION 3.8)
project("Mazeline Engine")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory("extern/GLew")
add_subdirectory("extern/libpng")
add_subdirectory("extern/zlib")
add_subdirectory("source/Engine")
add_subdirectory("source/Snake")
add_subdirectory("source/MVStudio")
add_subdirectory("source/Tests")