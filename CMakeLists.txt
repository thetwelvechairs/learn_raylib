###################
# PRE-REQUISITES: #
###################
#   - Conan       #
#   - Cmake       #
###################

cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "learn_raylib")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(${PROJECT_NAME} CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#### Set up Conan and build dependencies #### https://github.com/conan-io/cmake-conan ##################################
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
set(CONAN_SYSREQUIRES_MODE "enabled")
conan_cmake_configure(REQUIRES
        GENERATORS cmake cmake_paths cmake_find_package)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})
########################################################################################################################

# Dependencies
set(CUSTOMIZE_BUILD ON)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(raylib 4.0.0 QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
            message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
            set(PLATFORM "Raspberry Pi")
        endif()
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

file(GLOB H_FILES "src/*.h")
file(GLOB CPP_FILES "src/*.cpp")
file(GLOB INCLUDE "src/include/*.*/*.*/*.*")

add_executable(${PROJECT_NAME} ${H_FILES} ${CPP_FILES} ${INCLUDE})

target_link_libraries(${PROJECT_NAME}
        raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
