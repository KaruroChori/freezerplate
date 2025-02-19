#!/bin/bash
freezerplate ./files/ ./dist ./example-templ.xml
cd dist
./build.sh