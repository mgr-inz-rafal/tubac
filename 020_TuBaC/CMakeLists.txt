cmake_minimum_required(VERSION 3.1)
project(Tubac)
set(Boost_DEBUG 1)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_CXX_STANDARD 11)
include_directories("./include")
find_package(Boost 1.66.0 COMPONENTS program_options REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_executable(tubac
	src/basic_array.cpp
	src/context.cpp
        src/020_TuBaC.cpp
        src/number_type_integer.cpp
        src/command_line.cpp
        src/reactor.cpp
        src/synthesizer.cpp
        src/config.cpp
        src/runtime_base.cpp
        src/token_provider.cpp
        src/generator.cpp
        src/runtime_integer.cpp
        src/number_type_base.cpp
        src/stack.cpp
    )
    target_link_libraries(tubac ${Boost_LIBRARIES})
endif()
