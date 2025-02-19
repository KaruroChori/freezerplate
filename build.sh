#!/bin/bash
meson setup -Db_lto=true  --buildtype=release --reconfigure build/ --native-file ./platforms/default.ini
meson compile -C build
rm -rf ./dist/
./build/te4 ./template/ ./dist/
cp ./dist/main.cpp ./template.cpp
meson compile -C build
sudo meson install -C build