#!/bin/bash

curl -o fa.zip -L https://github.com/proscawards/force-awaken/releases/latest/download/Force-Awaken.Setup.zip &&
rm -rf $(ls -A |  grep -vE "(fa.zip)|(settings/*)") &&
unzip fa.zip -d "./" &&
rm -f fa.zip &&
taskkill //F //IM "Force_Awaken.exe" &&
start "Force_Awaken.exe" &&
[ -e NUL ] && rm -rf NUL
