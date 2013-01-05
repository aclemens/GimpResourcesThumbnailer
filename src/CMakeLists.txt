find_package(KDE4 REQUIRED)
include_directories(
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/AdobePhotoshop
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/Gimp
)

find_package(SharedMimeInfo REQUIRED)

file(GLOB loaders_PART_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/AdobePhotoshop/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/Gimp/*.cpp
)

set(gimpresourcesthumbnail_PART_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/thumbnailcreator.cpp
    ${loaders_PART_SRCS}
)

IF(KDE4_BUILD_TESTS)
    add_subdirectory(tests)
ENDIF(KDE4_BUILD_TESTS)


####################################################

set(mime_types_SRCS
    x-adobe-photoshop-brush.xml
    x-gimp-brush-animated.xml
    x-gimp-brush.xml
    x-gimp-gradient.xml
    x-gimp-palette.xml
    x-gimp-pattern.xml
)

kde4_add_plugin(gimpresourcesthumbnail ${gimpresourcesthumbnail_PART_SRCS})
target_link_libraries(gimpresourcesthumbnail ${KDE4_KIO_LIBS})

install(TARGETS gimpresourcesthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES gimpresourcesthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${mime_types_SRCS} DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
