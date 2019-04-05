#!/bin/sh

rm -f IN4200_PE_15301.zip
zip -r IN4200_PE_15301.zip . -x e".swo" "*.swp" ".*" "*.o" "*.py" ".tex" "doc/*" "web_graphs/web-NotreDame.txt"
