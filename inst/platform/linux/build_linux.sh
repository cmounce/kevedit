#!/bin/sh
set -e -x

SOURCE="$1"
if [ -z "$SOURCE" ]; then
    echo "USAGE: build_linux.sh <source.zip>"
    exit 1
fi

rm -rf /work/appdir
mkdir -p /work/appdir/KevEdit.AppDir

rm -rf /work/kevedit
mkdir /work/kevedit
cd /work/kevedit

unzip /vendor/$SOURCE

# /usr/local not included for some reason, skip bootstrap and set up manually
aclocal -I /usr/local/share/aclocal
autoconf
autoheader
automake --add-missing
./configure --prefix=/work/appdir/KevEdit.AppDir/usr CFLAGS='-O3'
make
make install

cp -a /platform/linux/kevedit.desktop /work/appdir/KevEdit.AppDir/
cp -a /work/kevedit/inst/icon512.png /work/appdir/KevEdit.AppDir/kevedit.png
cp /vendor/AppRun-x86_64 /work/appdir/KevEdit.AppDir/AppRun

mkdir -p /work/appdir/KevEdit.AppDir/usr/lib
cp -a /usr/local/lib/libSDL2*.so* /work/appdir/KevEdit.AppDir/usr/lib/
