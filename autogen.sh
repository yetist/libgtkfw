aclocal
autoheader
libtoolize -c --force
autoconf
automake -a -c
./configure --enable-maintainer-mode --prefix=/usr --sysconfdir=/etc $@
