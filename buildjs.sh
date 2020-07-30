#!/bin/bash
export LIBDE265_VERSION=1.0.4

emconfigure ./configure --disable-sse --disable-dec265 --disable-enc265 --disable-sherlock265
emmake make

