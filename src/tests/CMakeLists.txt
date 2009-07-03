SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../
)

#------------------------------------------------------------------------

SET(gimpbrushloadertest_SRCS gimpbrushloadertest.cpp
                         ../resourceloader.cpp
                         ../thumbnailcreator.cpp
                         ../gbrbrushloader.cpp
                         ../gihbrushloader.cpp
                         ../vbrbrushloader.cpp
                         ../abrbrushloader.cpp
)
KDE4_ADD_UNIT_TEST(gimpbrushloadertest ${gimpbrushloadertest_SRCS})
TARGET_LINK_LIBRARIES(gimpbrushloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )
