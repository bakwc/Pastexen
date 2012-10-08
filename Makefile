all:
	cd client && qmake && make

install:
	cd client && INSTALL_ROOT=$(DESTDIR) make install

clean:
	#cd client && qmake && INSTALL_ROOT=$(DESTDIR) make uninstall_target
	cd client && qmake && make clean
	rm -f client/Makefile
	rm -f client/PasteXen
