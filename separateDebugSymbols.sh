#/bin/bash

archivePath="$1"
archiveDebugPath="$2"
if [ -z "$1" ] || [ -z "$2" ]; then
	echo "Syntax: ./separateDebugSymbols.sh <archive-file> <archive-debug-file>"
	exit 1
fi

restoreDir=`pwd`
tempDir=`realpath _temp_dir`
rm -rf "$tempDir"

echo "*** Unpack $archivePath ..."
unpackOrgDir="$tempDir"/original
mkdir -p "$unpackOrgDir" || exit 1
tar -xjf "$archivePath" -C "$unpackOrgDir" || exit 1

echo "*** Create debug symbols ..."
debugSymbolDir="$tempDir"/debug
mkdir -p "$debugSymbolDir" || exit 1

cd "$unpackOrgDir" || exit 1
if [ -z "$OBJCOPY" ]; then
	OBJCOPY=objcopy
fi
if [ -z "$STRIP" ]; then
	STRIP=strip
fi

for f in `find -name "*.so"`; do
	libDir="`dirname "$f"`"
	libName="`basename "$f"`"
	debugPath="$debugSymbolDir/$f.debug"
	debugDir="$debugSymbolDir/$libDir"
	
	echo "Process $f ..."
	mkdir -p "$debugDir" || exit 1
	$OBJCOPY --only-keep-debug "$f" "$debugPath" || exit 1
	$STRIP --strip-debug --strip-unneeded "$f" || exit 1
	$OBJCOPY --add-gnu-debuglink="$debugPath" "$f" || exit 1
	chmod -x "$debugPath" || exit 1
done

echo "*** Pack $archivePath ..."
cd "$unpackOrgDir" || exit 1
tar -cjf "$restoreDir/$archivePath" . || exit 1

echo "*** Pack $archiveDebugPath ..."
cd "$debugSymbolDir" || exit 1
tar -cjf "$restoreDir/$archiveDebugPath" . || exit 1

cd "$restoreDir" || exit 1
rm -rf "$tempDir"
