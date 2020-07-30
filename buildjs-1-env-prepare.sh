#!/bin/bash

sudo apt update
sudo apt upgrade
sudo apt install python2
sudo apt-get install cmake
sudo apt-get install default-jre
sudo apt-get install automake-1.15
sudo apt install llvm

cd ..
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest