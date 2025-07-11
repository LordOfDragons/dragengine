#!/bin/bash

rm -rf page

cp -r ../page .

cp ../build/delauncher-web.{js,wasm,wasm.map,data} page
cp ../../../../build/release/lib/libdragengine.so.*.*.* page

ln -sf ../../src page
ln -sf ../../../shared page
ln -sf ../../../../dragengine page

cd page
emrun index.html
