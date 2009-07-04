SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../
)

#------------------------------------------------------------------------

SET(gimpbrushloadertest_SRCS
    gimpbrushloadertest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/resourceloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/gbrbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/gihbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/vbrbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/abrbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/patternloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../thumbnailcreator.cpp
)
KDE4_ADD_UNIT_TEST(gimpbrushloadertest ${gimpbrushloadertest_SRCS})
TARGET_LINK_LIBRARIES(gimpbrushloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )
