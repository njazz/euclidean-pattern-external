cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)
set(CMAKE_OSX_SYSROOT /)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

set(MAXPD_HEADER ${CMAKE_CURRENT_SOURCE_DIR} CACHE BOOL 1)

if(NOT TARGET MaxPdObject)
add_library(MaxPdObject INTERFACE)
target_sources(MaxPdObject INTERFACE ${MAXPD_HEADER}/MaxPdObject.hpp)
endif()

# works as add_library with all needed definitions
macro(add_maxpd_object NAME)

# 1. Pd object:
add_library(${NAME}_pd SHARED)

target_include_directories(${NAME}_pd PRIVATE ${MAXPD_HEADER})

target_compile_definitions(${NAME}_pd PRIVATE BUILD_PD_OBJECT=1)

if (WIN32)
    set_target_properties(${NAME}_pd PROPERTIES SUFFIX ".dll")
    set_target_properties(${NAME}_pd PROPERTIES PREFIX "")
    set_target_properties(${NAME}_pd PROPERTIES OUTPUT_NAME ${NAME})

    target_link_libraries(${NAME}_pd ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/pd_lib/pd.lib)
else()
    set_target_properties(${NAME}_pd PROPERTIES SUFFIX ".pd_darwin")
    set_target_properties(${NAME}_pd PROPERTIES PREFIX "")
    set_target_properties(${NAME}_pd PROPERTIES OUTPUT_NAME ${NAME})

    target_link_libraries(${NAME}_pd "-undefined dynamic_lookup")
endif()

target_include_directories(${NAME}_pd PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/pd_headers/")



set_target_properties(${NAME}_pd
        PROPERTIES

        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/pd-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/pd-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
)

add_custom_command(TARGET ${NAME}_pd 
	PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/help/${NAME}-help.pd ${CMAKE_SOURCE_DIR}/bin/pd-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${NAME}-help.pd
)

##########

# 2. Max object:

add_library(${NAME}_max MODULE)

SET_TARGET_PROPERTIES(${NAME}_max PROPERTIES BUNDLE TRUE)

target_include_directories(${NAME}_max PRIVATE ${MAXPD_HEADER})

target_compile_definitions(${NAME}_max PRIVATE BUILD_MAX_OBJECT=1)

if (WIN32)
    set_target_properties(${NAME}_max PROPERTIES SUFFIX ".mxe")
    set_target_properties(${NAME}_max PROPERTIES BUNDLE_EXTENSION "mxe")
    set_target_properties(${NAME}_max PROPERTIES PREFIX "")
    set_target_properties(${NAME}_max PROPERTIES OUTPUT_NAME ${NAME})

    target_compile_definitions(${NAME}_max PRIVATE WIN_VERSION)

    target_link_libraries(${NAME}_max
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/max-sdk/source/c74support/max-includes/x64/MaxAPI.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/max-sdk/source/c74support/max-includes/x64/maxcrt_p.lib"
        )
else()
    set_target_properties(${NAME}_max PROPERTIES SUFFIX ".mxo")
    set_target_properties(${NAME}_max PROPERTIES BUNDLE_EXTENSION "mxo")
    set_target_properties(${NAME}_max PROPERTIES PREFIX "")
    set_target_properties(${NAME}_max PROPERTIES OUTPUT_NAME ${NAME})

    target_link_libraries(${NAME}_max "-undefined dynamic_lookup")
endif()

target_include_directories(${NAME}_max PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/max-sdk/source/c74support/max-includes")

add_custom_command(TARGET ${NAME}_max
	PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/help/${NAME}.maxhelp ${CMAKE_SOURCE_DIR}/bin/max-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${NAME}.maxhelp
)

set_target_properties(${NAME}_max
        PROPERTIES

        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/max-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
)

target_link_libraries(${NAME}_pd MaxPdObject)
target_link_libraries(${NAME}_max MaxPdObject)

endmacro()

###

macro(add_maxpd_shared_library NAME)

# 1. Pd object:
add_library(${NAME}_pd SHARED)

target_include_directories(${NAME}_pd PRIVATE ${MAXPD_HEADER})

target_compile_definitions(${NAME}_pd PRIVATE BUILD_PD_OBJECT=1)

if (WIN32)
    target_link_libraries(${NAME}_pd ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/pd_lib/pd.lib)
else()
    target_link_libraries(${NAME}_pd "-undefined dynamic_lookup")
endif()

target_include_directories(${NAME}_pd PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/pd_headers/")

set_target_properties(${NAME}_pd
        PROPERTIES

        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/pd-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/pd-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
)


##########

# 2. Max object:

add_library(${NAME}_max SHARED)

target_include_directories(${NAME}_max PRIVATE ${MAXPD_HEADER})
target_compile_definitions(${NAME}_max PRIVATE BUILD_MAX_OBJECT=1)

if (WIN32)
    target_compile_definitions(${NAME}_max PRIVATE WIN_VERSION)

    target_link_libraries(${NAME}_max
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/max-sdk/source/c74support/max-includes/x64/MaxAPI.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/max-sdk/source/c74support/max-includes/x64/maxcrt_p.lib"
        )
else()
    target_link_libraries(${NAME}_max "-undefined dynamic_lookup")
endif()

target_include_directories(${NAME}_max PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/max-sdk/source/c74support/max-includes")

set_target_properties(${NAME}_max
        PROPERTIES

        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/max-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
)

target_link_libraries(${NAME}_pd MaxPdObject)
target_link_libraries(${NAME}_max MaxPdObject)

endmacro()
