# An example PKGBUILD script for Civetweb upstream, git version
# Rename to PKGBUILD to build via makepkg
_pkgname=civetweb
pkgname=$_pkgname-git
pkgver=v1.4.24.g73c40b6
pkgrel=1
pkgdesc="Small and quick-to-use web server; https/php/cgi support; MIT license - git development version"
arch=('i686' 'x86_64')
url="http://sourceforge.net/p/civetweb/"
license=('MIT')
groups=()
depends=()
makedepends=('git sed')
optdepends=('php-cgi: for php support')
provides=("$_pkgname")
conflicts=("$_pkgname")
backup=("etc/$_pkgname/$_pkgname.conf")
source=("$_pkgname::git+https://github.com/civetweb/civetweb.git")
md5sums=('SKIP')

pkgver() {
  cd "$srcdir/$_pkgname"
  git describe --tags | sed 's|-|.|g'
}

build() {
  cd "$srcdir/$_pkgname"
  make build WITH_IPV6=1
}

package() {
  cd "$srcdir/$_pkgname"
  make install PREFIX="$pkgdir/usr" SYSCONFDIR="$pkgdir/etc/local/$_pkgname"
  
  install -Dm644 "$srcdir/$_pkgname/distribution/arch/$_pkgname.service" "$pkgdir/usr/lib/systemd/system/$_pkgname.service"

  sed -i "s/^document_root [^\n]*/document_root \/srv\/http/g" "$pkgdir/etc/local/$_pkgname/$_pkgname.conf"
  sed -i "s/^# access_log_file/access_log_file \/var\/log\/$_pkgname\/access.log/g" "$pkgdir/etc/local/$_pkgname/$_pkgname.conf"
  sed -i "s/^# error_log_file/access_log_file \/var\/log\/$_pkgname\/error.log/g" "$pkgdir/etc/local/$_pkgname/$_pkgname.conf"
}

# vim:set ts=2 sw=2 et:
