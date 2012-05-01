if(APPLE)
	include_directories( /usr/X11/include)
endif(APPLE)

set(Polygon_SRCS
	Basic.h
    Common.cpp
    Common.h
    LineSegment.cpp
    Polygon.cpp
    Polygon.h
    PolygonList.cpp
    Screen.h
)

include_directories (
	${OPENGL_INCLUDE_DIR}
	${PictureCollage_SOURCE_DIR}
)

if(APPLE)
	# We are not using the Apple's framework version, but X11's
	include_directories( /usr/X11/include )
	link_directories( /usr/X11/lib )
	set (OPENGL_LIBRARIES GL GLU GLUT X11)
elseif(WIN32)
	set (ADDITIONAL_LIBRARIES winmm)
endif(APPLE)

add_library(Polygon
	${Polygon_SRCS}
)
set(exesrcs
    ${Polygon_SRCS}
    Main.cpp
)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#add_executable(Polygon
#    ${exesrcs} 
#)
target_link_libraries(
    Polygon
    Box2D
    freeglut
    glui
    ${ADDITIONAL_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
