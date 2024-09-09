#/bin/bash

buildPackage=false
for arg in "$*"; do
  case "$arg" in
  --build-package)
    buildPackage=true ;;
  esac
done

umask 0
cd /sources/dragengine

apt update -y -q \
  && apt-get -y -q install software-properties-common \
  && add-apt-repository -y -u ppa:rpluess/dragondreams \
  && apt-get -y -q upgrade \
  && apt-get -y -q install libdscript-dev || exit 1

export SCONSFLAGS="-j 8"

git clean -dfx || exit 1

fetchExternals() {
  scons lib_eossdk_fetch lib_fox_fetch lib_liburing_fetch \
    lib_modio_fetch lib_openxr_fetch lib_steamsdk_fetch \
    lib_libapng_fetch || exit 1
}

writeIncludeBinaries() {
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
}

cleanScons() {
  find -type d -name "__pycache__" | xargs -- rm -rf
  rm -f config.log
  rm -f build.log
  rm -rf .sconf_temp
  rm -f .sconsign.dblite
}

fetchExternals
writeIncludeBinaries
cleanScons

rm -rf /sources/dragengine_*.orig.tar.gz
rm -rf /sources/dragengine_*-ppa*

# required since noble or git fails
git config --global --add safe.directory /sources/dragengine

if [ $buildPackage = true ]; then
  # this is no more working since noble. the build is just not started.
  #gbp buildpackage --git-debian-branch=debian --git-upstream-tree=debian --git-ignore-new --git-force-create || exit 1
  ./debian/rules override_dh_auto_build || exit 1
  ./debian/rules override_dh_auto_install || exit 1
  ./debian/rules override_dh_shlibdeps || exit 1
else
  gbp export-orig --upstream-tree=debian --force-create || exit 1
fi

# gbp does not include the downloaded files in the source archive. fix it
FILE=`cd .. && dir -1 dragengine_*.orig.tar.gz`
FILENOEXT=`echo $FILE | sed -e "s/.orig.tar.gz//" | sed -e "s/_/-/"`

gunzip ../dragengine_*.orig.tar.gz || exit 1
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
  `dir -1 extern/libapng/libpng-*tar.bz2` || exit 1
gzip ../dragengine_*.orig.tar || exit 1

git clean -dfx || exit 1

# above clean also kills the prefetched externals and include binaries file.
# debuild now seems to need this suddenly
fetchExternals
writeIncludeBinaries
cleanScons

debuild -S -sa || exit 1

dput ppa:rpluess/dragondreams /sources/dragengine_*_source.changes || exit 1
