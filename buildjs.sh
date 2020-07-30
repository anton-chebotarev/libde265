#!/bin/bash
export LIBDE265_VERSION=1.0.4

chmod 777 ./configure
emconfigure ./configure --disable-sse --disable-dec265 --disable-enc265 --disable-sherlock265
emmake make

