cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)
set(CMAKE_OSX_SYSROOT /)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

add_subdirectory(../MaxPdObject "MaxPdObject")

add_compile_options(-Wall)
# mostly for puredata sources:
#add_compile_options(-Wno-unused-parameter)

if(POLICY CMP0074)
    #policy for <PackageName>_ROOT variables
    cmake_policy(SET CMP0074 NEW)
endif()

project(euclidean_pattern_object)

set(SRC

    src/EuclideanPattern.cpp
    src/EuclideanPattern.hpp

    src/Object.cpp
)

add_maxpd_object(euclidean_pattern)

target_sources(euclidean_pattern_pd PRIVATE ${SRC} )

target_include_directories(euclidean_pattern_pd PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../euclidean_pattern/src")


#target_link_libraries(organ_core_pd mqtt-object)

###

target_sources(euclidean_pattern_max PRIVATE ${SRC} )

target_include_directories(euclidean_pattern_max PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../euclidean_pattern/src")


#target_link_libraries(organ_core_max mqtt-object)

###

