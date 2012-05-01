set(IO_SRCS
    Picture.h
    PictureIO.h
    Picture.cpp
    PictureIO.cpp
    )

include_directories (
    ${PROJECT_SOURCE_DIR}
    ${BOOST_ROOT}
    )
FIND_PACKAGE(Boost 1.3.4 REQUIRED)
add_library(IO
    ${IO_SRCS}
    )
target_link_libraries(IO
    json_spirit
    ${Boost_LIBRARIES}
    )
