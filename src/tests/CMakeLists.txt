SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../
)

#------------------------------------------------------------------------

FILE(GLOB loaders_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../loaders/*.cpp
)

#------------------------------------------------------------------------

SET(gimpbrushloadertest_SRCS
    gimpbrushloadertest.cpp
    ${loaders_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../thumbnailcreator.cpp
)

KDE4_ADD_UNIT_TEST(gimpbrushloadertest ${gimpbrushloadertest_SRCS})
TARGET_LINK_LIBRARIES(gimpbrushloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )

#------------------------------------------------------------------------

SET(resourceloadertest_SRCS
    resourceloadertest.cpp
    ${loaders_SRCS}
)

KDE4_ADD_UNIT_TEST(resourceloadertest ${resourceloadertest_SRCS})
TARGET_LINK_LIBRARIES(resourceloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )

#------------------------------------------------------------------------

SET(vbrbrushloadertest_SRCS
    vbrbrushloadertest.cpp
    ${loaders_SRCS}
)

KDE4_ADD_UNIT_TEST(vbrbrushloadertest ${vbrbrushloadertest_SRCS})
TARGET_LINK_LIBRARIES(vbrbrushloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )

#------------------------------------------------------------------------

SET(gradientloadertest_SRCS
    gradientloadertest.cpp
    ${loaders_SRCS}
)

KDE4_ADD_UNIT_TEST(gradientloadertest ${gradientloadertest_SRCS})
TARGET_LINK_LIBRARIES(gradientloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )

#------------------------------------------------------------------------

SET(paletteloadertest_SRCS
    paletteloadertest.cpp
    ${loaders_SRCS}
)

KDE4_ADD_UNIT_TEST(paletteloadertest ${paletteloadertest_SRCS})
TARGET_LINK_LIBRARIES(paletteloadertest
                      ${KDE4_KIO_LIBS}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTTEST_LIBRARY}
                      )
