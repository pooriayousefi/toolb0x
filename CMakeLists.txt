cmake_minimum_required(VERSION 3.20)
project(toolb0x
    VERSION 1.0.0
    DESCRIPTION "Modern C++ header-only library"
    HOMEPAGE_URL "https://github.com/pooriayousefi/toolb0x"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only library target
add_library(toolb0x INTERFACE)
target_include_directories(toolb0x INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Set interface properties
target_compile_features(toolb0x INTERFACE cxx_std_20)

# Platform-specific compile definitions
if(WIN32)
    target_compile_definitions(toolb0x INTERFACE _WIN32_WINNT=0x0601)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS toolb0x
    EXPORT toolb0xTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/toolb0x.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    OPTIONAL
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/toolb0x.ixx
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    OPTIONAL
)

# Export targets
install(EXPORT toolb0xTargets
    FILE toolb0xTargets.cmake
    NAMESPACE toolb0x::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/toolb0x
)

# Create package config
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolb0xConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/toolb0xConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/toolb0x
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/toolb0xConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/toolb0xConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/toolb0xConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/toolb0x
)

# Configuration summary
message(STATUS "toolb0x Configuration Summary:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
