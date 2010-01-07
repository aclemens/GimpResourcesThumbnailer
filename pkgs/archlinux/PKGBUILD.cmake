# Contributor: Andi Clemens <andi.clemens@gmx.net>

pkgname=gimpresourcesthumbnailer
pkgver=@GRT_VERSION_STRING@
pkgrel=1
pkgdesc="Plugin for KDE to display GIMP resource files like brushes and patterns as thumbnails."
arch=('i686' 'x86_64')
url="@PKG_HOME_URL@"
depends=('kdebase-workspace')
makedepends=('cmake' 'automoc4')
source=(@PKG_SOURCE_URL@)
install="gimpresourcesthumbnailer.install"

license=('GPL')

build() {
  cd $srcdir/@PKG_NAME@-$pkgver
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX='/usr' -DKDE4_BUILD_TESTS=OFF ..
  make || return 1
  make DESTDIR=$pkgdir install
}
