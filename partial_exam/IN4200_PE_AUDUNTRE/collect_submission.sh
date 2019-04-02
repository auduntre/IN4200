#!/bin/sh

rm -f IN4200_PE_15301.zip
zip -r IN4200_PE_15301.zip . -x ".o" ".exe" "*.swp" "doc/*" "report/*.tex"
