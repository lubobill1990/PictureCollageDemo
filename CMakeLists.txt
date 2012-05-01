cmake_minimum_required(VERSION 2.4)

project(PictureCollage)

if(UNIX)
    set(PICTURECOLLAGE_INSTALL_BY_DEFAULT ON)
else(UNIX)
	set(PICTURECOLLAGE_INSTALL_BY_DEFAULT OFF)
endif(UNIX)

option(PICTURECOLLAGE_INSTALL "Install Box2D libs, includes, and CMake scripts" ${PICTURECOLLAGE_INSTALL_BY_DEFAULT})
option(PICTURECOLLAGE_INSTALL_DOC "Install Box2D documentation" OFF)
option(PICTURECOLLAGE_BUILD_SHARED "Build Box2D shared libraries" OFF)
option(PICTURECOLLAGE_BUILD_STATIC "Build Box2D static libraries" ON)
option(PICTURECOLLAGE_BUILD_EXAMPLES "Build Box2D examples" ON)

option(BOX2D_BUILD_SHARED "Build Box2D shared libraries" OFF)
option(BOX2D_BUILD_STATIC "Build Box2D static libraries" ON)

SET(BOX2D_VERSION 2.1.0)
SET(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
FIND_PACKAGE(Boost)
#IF(NOT Boost_FOUND)
    SET(BOOST_ROOT "C:/Users/Lubo/Documents/Visual Studio 2010/Libraries/boost_1_49_0")
    MESSAGE(STATUS "SET BOOST MANUALLY")
    #ENDIF(NOT Boost_FOUND)
    SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModule;${CMAKE_MODULE_PATH}")
# The Box2D library.
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT)

MESSAGE(STATUS "${Boost_INCLUDE_DIRS}")
SET(ENV{CMAKE_CURRENT_INCLUDE_DIR} 
    ${PROJECT_SOURCE_DIR}
    ${Boost_ROOT})
SET(ENV{CMAKE_CURRENT_LIBRARY_PATH} "./lib")

add_subdirectory(Box2D)
add_subdirectory(freeglut)
add_subdirectory(glui)
add_subdirectory(json_spirit)
add_subdirectory(IO)
add_subdirectory(Polygon)
add_subdirectory(PictureCollage)

