#!/bin/sh

aclocal
autoconf
autoheader
touch README.md AUTHORS NEWS ChangeLog
ln -sfv README.md README
automake -a

if [ -f "configure" ]; then
	./configure --prefix=/opt/xsmaug --with-gnu-ld --enable-shared
fi;

