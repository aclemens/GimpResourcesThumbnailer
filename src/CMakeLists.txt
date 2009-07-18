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
)

set(gimpbrushthumbnail_PART_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/thumbnailcreator.cpp
    ${loaders_PART_SRCS}
)

add_subdirectory(tests)

####################################################

set(mime_types_SRCS
    x-gimp-brush.xml
    x-gimp-brush-animated.xml
    x-gimp-pattern.xml
    x-adobe-photoshop-brush.xml
)

kde4_add_plugin(gimpbrushthumbnail ${gimpbrushthumbnail_PART_SRCS})
target_link_libraries(gimpbrushthumbnail ${KDE4_KIO_LIBS})

install(TARGETS gimpbrushthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES gimpbrushthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${mime_types_SRCS} DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
