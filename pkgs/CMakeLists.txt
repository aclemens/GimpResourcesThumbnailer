SET(PKG_NAME        "GimpResourcesThumbnailer")
set(PKG_ID          "107128")
SET(PKG_HOME_URL    "http://www.kde-apps.org/content/show.php/${PKG_NAME}?content=${PKG_ID}")
SET(PKG_SOURCE_URL  "http://www.kde-apps.org/CONTENT/content-files/${PKG_ID}-${PKG_NAME}-$pkgver.tar.gz")

# ==================================================================================================
# Generate an Archlinux PKGBUILD

CONFIGURE_FILE(archlinux/PKGBUILD.cmake archlinux/PKGBUILD)

# ==================================================================================================

