find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES}
                     ${CMAKE_CURRENT_SOURCE_DIR}/loaders
)

find_package(SharedMimeInfo REQUIRED)

set(loaders_PART_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/resourceloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/abrbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/gbrbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/gihbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/patternloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/vbrbrushloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders/gradientloader.cpp
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
    x-gimp-brush.xml
    x-gimp-brush-animated.xml
    x-gimp-pattern.xml
    x-gimp-gradient.xml
    x-adobe-photoshop-brush.xml
)

kde4_add_plugin(gimpresourcesthumbnail ${gimpresourcesthumbnail_PART_SRCS})
target_link_libraries(gimpresourcesthumbnail ${KDE4_KIO_LIBS})

install(TARGETS gimpresourcesthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES gimpresourcesthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${mime_types_SRCS} DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
