cmake_minimum_required(VERSION 3.14.5)
project(http_get LANGUAGES CXX VERSION 0.1.0)

# -------------------------------------------
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
    REQUIRES
        cpprestsdk/2.10.14@bincrafters/stable
        fmt/5.3.0@bincrafters/stable
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
)
# -------------------------------------------

add_executable(http_get main.cpp)
target_link_libraries(http_get CONAN_PKG::cpprestsdk CONAN_PKG::fmt)
