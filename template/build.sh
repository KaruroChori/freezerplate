#!/bin/bash
meson setup -Db_lto=true  --buildtype=release --reconfigure build/
sudo meson install -C build