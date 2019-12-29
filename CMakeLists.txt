cmake_minimum_required(VERSION 3.14.5)
project(bing_show LANGUAGES CXX VERSION 0.1.0)

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
        fmt/6.0.0@bincrafters/stable
        opencv/4.1.1@conan/stable
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
    OPTIONS
        boost:without_atomic=True
        boost:without_chrono=True
        boost:without_container=True
        boost:without_context=True
        boost:without_contract=True
        boost:without_coroutine=True
        boost:without_date_time=True
        boost:without_exception=False
        boost:without_fiber=True
        boost:without_filesystem=True
        boost:without_graph=True
        boost:without_graph_parallel=True
        boost:without_iostreams=True
        boost:without_locale=True
        boost:without_log=True
        boost:without_math=True
        boost:without_mpi=True
        boost:without_program_options=True
        boost:without_python=True
        boost:without_random=True
        boost:without_regex=True
        boost:without_serialization=True
        boost:without_stacktrace=True
        boost:without_system=True
        boost:without_test=True
        boost:without_thread=True
        boost:without_timer=True
        boost:without_type_erasure=True
        boost:without_wave=True
        opencv:gtk=3
        opencv:contrib=True
        opencv:gstreamer=False
        opencv:protobuf=False
        harfbuzz:with_glib=False
)
# -------------------------------------------

add_executable(bing_show main.cpp)
target_link_libraries(bing_show CONAN_PKG::cpprestsdk CONAN_PKG::fmt CONAN_PKG::opencv)
