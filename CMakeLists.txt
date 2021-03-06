PROJECT(GimpResourcesThumbnailer)
FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE (KDE4Defaults)
INCLUDE(MacroOptionalAddSubdirectory)

# ==================================================================================================
# Information to update before to release this package.

# GimpResourcesThumbnailer version
SET(GRT_MAJOR_VERSION "2")
SET(GRT_MINOR_VERSION "1")
SET(GRT_PATCH_VERSION "2")

# Suffix to add at end of version string. Examples:
# "-git"
# "-beta1"
# "-beta2"
# "-rc"
# "" : final release. Can be used in production.
SET(GRT_SUFFIX_VERSION "")

SET(GRT_VERSION_SHORT
    "${GRT_MAJOR_VERSION}.${GRT_MINOR_VERSION}.${GRT_PATCH_VERSION}"
)

SET(GRT_VERSION_STRING
    "${GRT_VERSION_SHORT}${GRT_SUFFIX_VERSION}"
)

MESSAGE(STATUS "")

# ==================================================================================================
# API documentation generation

MACRO_OPTIONAL_FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen found... YES (optional)")
    MESSAGE(STATUS "")
    MESSAGE(STATUS " * API documentation can be build with the 'doc' make target ")
    MESSAGE(STATUS "")
ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen found... NO (optional)")
    MESSAGE(STATUS "")
    MESSAGE(STATUS " * You will not be able to to generate the API documentation.")
    MESSAGE(STATUS "")
ENDIF(DOXYGEN_FOUND)

IF(DOXYGEN_FOUND)

    SET(DOC_DIR ${CMAKE_BINARY_DIR}/doc)
    SET(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
    SET(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)
    SET(WARNFILE ${CMAKE_BINARY_DIR}/doxygen-warn.log)

    CONFIGURE_FILE(Doxyfile.cmake ${DOXYFILE})

    ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

ENDIF(DOXYGEN_FOUND)

# ==================================================================================================

ADD_DEFINITIONS (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

ADD_SUBDIRECTORY( src )
