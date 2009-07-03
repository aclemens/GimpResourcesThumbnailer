find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} )
add_subdirectory(tests)

find_package(SharedMimeInfo REQUIRED)

set(gimpbrushthumbnail_PART_SRCS
    resourceloader.cpp
    thumbnailcreator.cpp
    abrbrushloader.cpp 
    gbrbrushloader.cpp 
    gihbrushloader.cpp 
    patternloader.cpp 
    vbrbrushloader.cpp)

kde4_add_plugin(gimpbrushthumbnail ${gimpbrushthumbnail_PART_SRCS})

target_link_libraries(gimpbrushthumbnail ${KDE4_KIO_LIBS})

install(TARGETS gimpbrushthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES gimpbrushthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES x-gimp-brush-thumbnailer.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
