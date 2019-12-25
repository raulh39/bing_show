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
        zlib/1.2.11@conan/stable
        bzip2/1.0.8@conan/stable
        cpprestsdk/2.10.14@bincrafters/stable
        fmt/6.0.0@bincrafters/stable
        imagemagick/7.0.8-44@bincrafters/stable
        freetype/2.10.0@bincrafters/stable
        fontconfig/2.13.91@conan/stable
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
)
# -------------------------------------------

add_executable(http_get main.cpp)
target_link_libraries(http_get CONAN_PKG::cpprestsdk CONAN_PKG::fmt CONAN_PKG::imagemagick CONAN_PKG::fontconfig)
