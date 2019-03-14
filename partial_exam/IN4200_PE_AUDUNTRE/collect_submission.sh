#!/bin/sh

rm -f IN4200_PE_AUDUNTRE.zip
zip -r IN4200_PE_AUDUNTRE.zip . -x ".o" ".exe" "*.swp" "doc/*" "report/*.tex"
