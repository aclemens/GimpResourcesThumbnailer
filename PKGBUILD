# Contributor: Andi Clemens <andi.clemens@gmx.net>

pkgname=gimpresourcesthumbnailer
pkgver=2.0.1-git
pkgrel=1
pkgdesc="Plugin for KDE to display GIMP resource files like brushes and patterns as thumbnails."
arch=('i686' 'x86_64')
url="http://www.kde-apps.org/content/show.php/GimpResourcesThumbnailer?content=107128"
depends=('kdebase-workspace')
makedepends=('cmake' 'automoc4')
source=(http://www.kde-apps.org/CONTENT/content-files/107128-GimpResourcesThumbnailer-$pkgver.tar.gz)

license=('GPL')

build() {
  cd $srcdir/GimpResourcesThumbnailer-$pkgver
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX='/usr' -DKDE4_BUILD_TESTS=OFF ..
  make || return 1
  make DESTDIR=$pkgdir install
}
