#!/bin/bash

rm -rf page
cp -r ../page .
cp ../build/delauncher-web.{js,wasm} page
cp ../../../../build/release/lib/libdragengine.so.*.*.* page
cp -r ../../../../build/release/lib/dragengine/modules/libdemodules.so page
cp -r ../../../../build/release/share/dragengine/dragengine-*.deal page
cd page
emrun index.html
