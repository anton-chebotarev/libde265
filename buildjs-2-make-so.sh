#!/bin/bash

source ../emsdk/emsdk_env.sh

chmod 777 ./configure

emconfigure ./configure --disable-sse --disable-dec265 --disable-sherlock265
emmake make