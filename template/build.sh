#!/bin/bash
meson setup -Db_lto=true  --buildtype=release --reconfigure build/
meson compile -C build
sudo meson install -C build