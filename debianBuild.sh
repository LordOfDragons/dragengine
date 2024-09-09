#/bin/bash

umask 0
cd /sources/dragengine

apt update -y -q \
  && apt-get -y -q install software-properties-common \
  && add-apt-repository -y -u ppa:rpluess/dragondreams \
  && apt-get -y -q upgrade \
  && apt-get -y -q install libdscript-dev

export SCONSFLAGS="-j 8"

git clean -dfx

scons lib_eossdk_fetch lib_fox_fetch lib_liburing_fetch \
  lib_modio_fetch lib_openxr_fetch lib_steamsdk_fetch \
  lib_libapng_fetch

FILE=debian/source/include-binaries
echo "detesting/data/content/model/box/box2.demodel" >$FILE
echo "detesting/data/content/model/box/box.demodel" >>$FILE
echo "detesting/data/content/image/icon64.png" >>$FILE
echo "extern/eossdk/eossdk.zip" >>$FILE
echo "extern/eossdk/eossdk_bin_linux.tar.xz" >>$FILE
echo `dir -1 extern/fox/fox-*.tar.bz2` >>$FILE
echo `dir -1 extern/liburing/liburing-liburing-*.tar.bz2` >>$FILE
echo "extern/modio/modio-sdk-ext.tar.xz" >>$FILE
echo "extern/modio/modio-sdk.tar.xz" >>$FILE
echo `dir -1 extern/openxr/OpenXR-SDK-release-*.tar.xz` >>$FILE
echo `dir -1 extern/steamsdk/steamsdk160.tar.xz` >>$FILE
echo "extern/mingw/mingw_stdthreads.tar.bz2" >>$FILE
echo `dir -1 extern/libapng/libpng-*tar.bz2` >>$FILE

rm -rf /sources/dragengine_*.orig.tar.gz
rm -rf /sources/dragengine_*-ppa*

# required since noble or git fails
git config --global --add safe.directory /sources/dragengine

gbp export-orig --upstream-tree=debian --force-create

# gbp does not include the downloaded files in the source archive. fix it
FILE=`cd .. && dir -1 dragengine_*.orig.tar.gz`
FILENOEXT=`echo $FILE | sed -e "s/.orig.tar.gz//" | sed -e "s/_/-/"`

gunzip ../dragengine_*.orig.tar.gz
tar --transform "s@^\(extern.*\)@$FILENOEXT/\\1@" -rf ../dragengine_*.orig.tar \
  extern/eossdk/eossdk.zip \
  extern/eossdk/eossdk_bin_linux.tar.xz \
  `dir -1 extern/fox/fox-*.tar.bz2` \
  `dir -1 extern/liburing/liburing-liburing-*.tar.bz2` \
  extern/modio/modio-sdk-ext.tar.xz \
  extern/modio/modio-sdk.tar.xz \
  `dir -1 extern/openxr/OpenXR-SDK-release-*.tar.xz` \
  `dir -1 extern/steamsdk/steamsdk160.tar.xz` \
  extern/mingw/mingw_stdthreads.tar.bz2 \
  `dir -1 extern/libapng/libpng-*tar.bz2`
gzip ../dragengine_*.orig.tar

find -type d -name "__pycache__" | xargs -- rm -rf
rm -f config.log
rm -f build.log
rm -rf .sconf_temp
rm -f .sconsign.dblite

debuild -S -sa

dput ppa:rpluess/dragondreams /sources/dragengine_*_source.changes
