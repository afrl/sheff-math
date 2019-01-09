cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0048 NEW)
project(SIMPLEMATH VERSION 1.0.0 LANGUAGES CXX)

# Options
option(SIMPLEMATH_BuildDev "Build the project in the dev folder. (intended for debugging this project by devs)" OFF)
option(SIMPLEMATH_BuildTests "Build the unit tests." OFF)

##################
# Checking input #
##################
 
include(${CMAKE_SOURCE_DIR}/cmake/CommonInputChecks.cmake)

########################
# Declare dependencies #
########################


find_package(UTILITY CONFIG REQUIRED HINTS "${CMAKE_SOURCE_DIR}/../utility/build" "${CMAKE_SOURCE_DIR}/../utility/install_dir")
find_package(Eigen3 3.3.0 CONFIG REQUIRED HINTS "${CMAKE_SOURCE_DIR}/../third_party/eigen/install_dir" "${CMAKE_SOURCE_DIR}/../eigen/install_dir")


################################
# Setting up SIMPLEMATH target #
################################

# Set the variables for SIMPLEMATH
set(SIMPLEMATH_INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(SIMPLEMATH_INCLUDE_SUB ${SIMPLEMATH_INCLUDE}/simple_math)
set(SIMPLEMATH_SRC ${CMAKE_SOURCE_DIR}/source)
file (GLOB_RECURSE SIMPLEMATH_HEADER_LIST "${SIMPLEMATH_INCLUDE}/*.h")
file (GLOB_RECURSE SIMPLEMATH_SOURCE_LIST "${SIMPLEMATH_SRC}/*.h" "${SIMPLEMATH_SRC}/*.cpp")
    
# Create target
add_library(simplemath STATIC)
# Add the header includes and headers to the library
target_include_directories(simplemath PUBLIC
    $<BUILD_INTERFACE:${SIMPLEMATH_INCLUDE}>
    $<INSTALL_INTERFACE:include>)
# Add headers in the include folder as PUBLIC so downstream
# targets have access to them
target_sources(simplemath PUBLIC ${SIMPLEMATH_HEADER_LIST})
# Add headers and sources in the src folder as PRIVATE as they
# will not be needed downstream
target_sources(simplemath PRIVATE ${SIMPLEMATH_SOURCE_LIST})
# Add an alias so that a library can be used inside the build tree, eg when testing
add_library(SimpleMath::SimpleMath ALIAS simplemath)
# Add C++ 14 flag
target_compile_features(simplemath PUBLIC cxx_std_14)
# Link dependencies
target_link_libraries(simplemath PUBLIC Utility::Utility)
target_link_libraries(simplemath PUBLIC Eigen3::Eigen)


#################################
# Add dev and unit test targets #
#################################

# Build dev
if(SIMPLEMATH_BuildDev)
    add_subdirectory(dev)
endif()
    
# Build tests
if(SIMPLEMATH_BuildTests)
    include(CTest)
    enable_testing()
    add_subdirectory(unit_tests)
endif()

################
# Installation #
################

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SimpleMath)

# Install the simplemath target (library and headers)
install(
    TARGETS simplemath
    EXPORT SimpleMathTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)
# This is required so that the exported target has the name SimpleMath and not simplemath
set_target_properties(simplemath PROPERTIES EXPORT_NAME SimpleMath)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleMathConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create the package config file for find_package(SimpleMath)
configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/SimpleMathConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleMathConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install the config and config version files
# Note: if you write custom find modules, add them here
# Refer to https://github.com/pabloariasal/modern-cmake-sample/blob/cfda7b51ff915409fd61d4f2e75b63032bedb182/libjsonutils/CMakeLists.txt
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleMathConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleMathConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleMathTargets.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

###########
# Exports #
###########

# Export just the simplemath target (not the dev, tests, etc.)
export(
    TARGETS simplemath
    FILE ${CMAKE_CURRENT_BINARY_DIR}/SimpleMathTargets.cmake
    NAMESPACE SimpleMath::
)

# Register package in user's package registry
export(PACKAGE SimpleMath)
