#!/bin/bash

rm -rf page
cp -r ../page .
cp ../build/delauncher-web.{js,wasm,data} page
cp ../../../../build/release/lib/libdragengine.so.*.*.* page
cd page
emrun index.html
