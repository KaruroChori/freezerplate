#!/bin/bash
# Run to generate the parametric template based on the content in `dist`
freezerplate ./files/ ./dist ./example-templ.xml
cd dist
./build.sh