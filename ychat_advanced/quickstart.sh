#!/bin/sh

./configure &&
make &&
cd mods &&
./compile.sh &&
cd - &&
echo Starting yChat
./ychat
