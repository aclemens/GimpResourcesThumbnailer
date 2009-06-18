SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../
)

#------------------------------------------------------------------------

SET(brushloadertest_SRCS brushloadertest.cpp
)
KDE4_ADD_UNIT_TEST(brushloadertest ${brushloadertest_SRCS})
TARGET_LINK_LIBRARIES(brushloadertest
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )

