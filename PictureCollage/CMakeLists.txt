# Some flags for Freeglut and GLUI.
add_definitions( -DFREEGLUT_EXPORTS -DFREEGLUT_STATIC -D_CRT_SECURE_NO_WARNINGS )

# Define the framework files.
set(PictureCollage_Framework_SRCS
	Framework/Main.cpp
	Framework/Render.cpp
	Framework/Render.h
	Framework/Test.cpp
	Framework/Test.h
)

#define the test files.
set(PictureCollage_Tests_SRCS
    Tests/common.h
    Tests/Common.cpp    
    Tests/PolyShapes.h
    Tests/PolyShapes.cpp
    Tests/FixtureData.cpp  
    Tests/PicsContactFilter.h 
    Tests/PicsContactFilter.cpp
    Tests/BodyTypes.h   
    Tests/BodyData.h
    Tests/FixtureData.h    
    Tests/HandleLayoutResult.h 
    Tests/HandleLayoutResult.cpp   
    Tests/TestEntries.cpp
)

set(PictureCollage_Texture_SRCS
	Texture/Texture.cpp
	Texture/Texture.h
)

# These are used to create visual studio folders.
source_group(Framework FILES ${PictureCollage_Framework_SRCS})
source_group(Tests FILES ${PictureCollage_Tests_SRCS})
source_group(Texture FILES ${PictureCollage_Texture_SRCS})

include_directories (
	${OPENGL_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
    ${BOOST_ROOT}
)

if(APPLE)
	# We are not using the Apple's framework version, but X11's
	include_directories( /usr/X11/include )
	link_directories( /usr/X11/lib )
	set (OPENGL_LIBRARIES GL GLU GLUT X11)
endif(APPLE)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
add_executable(PictureCollage
    ${PictureCollage_Framework_SRCS}
    ${PictureCollage_Tests_SRCS}
    ${PictureCollage_Texture_SRCS}
    )

target_link_libraries (
    PictureCollage
    Box2D
    freeglut
    glui
    IO
    ${OPENGL_LIBRARIES}
    )
