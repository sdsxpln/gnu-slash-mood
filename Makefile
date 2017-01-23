APP=gnu-slash-mood
VERSION=1.00
RELEASE=1
ARCH_TYPE=`uname -m`
PREFIX?=/usr/local

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o ${APP} src/*.c -I/usr/local/include/libbbb -I/usr/local/include/libgnusocial -Isrc -lm -lcurl -lbbb -lgnusocial
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o ${APP} src/*.c -I/usr/local/include/libbbb -I/usr/local/include/libgnusocial -Isrc -lm -lcurl -lbbb -lgnusocial
source:
	tar -cvf ../${APP}_${VERSION}.orig.tar ../${APP}-${VERSION} --exclude-vcs
	gzip -f9n ../${APP}_${VERSION}.orig.tar
install:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}${PREFIX}
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 755 -s ${APP} ${DESTDIR}${PREFIX}/bin
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}${PREFIX}/share/man/man1
uninstall:
	rm -f ${PREFIX}/share/man/man1/${APP}.1.gz
	rm -rf ${PREFIX}/share/${APP}
	rm -f ${PREFIX}/bin/${APP}
clean:
	rm -f ${APP} \#* \.#* src/*.plist tests/*.plist tests/tests
