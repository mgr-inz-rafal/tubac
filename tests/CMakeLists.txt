cmake_minimum_required(VERSION 3.1)
project(Tubac_Tests)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(REQUIRE_CPP_17_FLAGS "-std=c++17")
add_definitions(${REQUIRE_CPP_17_FLAGS})
include_directories("./include")
find_package(Threads)
find_package(Boost 1.66.0 COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_executable(tubac_tests
        src/process_executor.cpp
        src/process_group_executor.cpp
        src/tests.cpp
    )
    target_link_libraries(tubac_tests ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
