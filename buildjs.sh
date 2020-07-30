#!/bin/bash

###git clone https://github.com/emscripten-core/emsdk.git
###cd emsdk
###git pull
###./emsdk install latest
###./emsdk activate latest

###sudo apt update
###sudo apt upgrade
###sudo apt install python2
###sudo apt-get install cmake
###sudo apt-get install default-jre
###sudo apt-get install automake-1.15
###sudo apt install llvm

###git restore .
###git pull
###chmod 777 ./configure

###source ../emsdk/emsdk_env.sh

echo "Building shared library..."
emconfigure ./configure --disable-sse --disable-dec265 --disable-enc265 --disable-sherlock265
emmake make

export LIBDE265_VERSION=1.0.4

export TOTAL_MEMORY=16777216

export EXPORTED_FUNCTIONS="[ \
    '_de265_get_version', \
    '_de265_get_version_number', \
    '_de265_get_error_text', \
    '_de265_isOK', \
    '_de265_set_verbosity', \
    '_de265_get_image_width', \
    '_de265_get_image_height', \
    '_de265_get_chroma_format', \
    '_de265_get_bits_per_pixel', \
    '_de265_get_image_plane', \
    '_de265_get_image_plane_user_data', \
    '_de265_get_image_PTS', \
    '_de265_get_image_user_data', \
    '_de265_set_image_user_data', \
    '_de265_new_decoder', \
    '_de265_free_decoder', \
    '_de265_push_data', \
    '_de265_push_end_of_NAL', \
    '_de265_push_end_of_frame', \
    '_de265_push_NAL', \
    '_de265_flush_data', \
    '_de265_get_number_of_input_bytes_pending', \
    '_de265_get_number_of_NAL_units_pending', \
    '_de265_decode', \
    '_de265_reset', \
    '_de265_peek_next_picture', \
    '_de265_get_next_picture', \
    '_de265_release_next_picture', \
    '_de265_get_warning', \
    '_de265_get_highest_TID', \
    '_de265_get_current_TID', \
    '_de265_set_limit_TID', \
    '_de265_set_framerate_ratio', \
    '_de265_change_framerate', \
    '_de265_set_parameter_bool', \
    '_de265_set_parameter_int', \
    '_de265_get_parameter_bool'
]"

export LIBRARY_FUNCTIONS="[ \
    'memcpy', \
    'memset', \
    'malloc', \
    'free'
]"

echo "Compiling into asm-js..."
emcc libde265-${LIBDE265_VERSION}/libde265/.libs/libde265.so \
    -s NO_EXIT_RUNTIME=1 \
    -s TOTAL_MEMORY=${TOTAL_MEMORY} \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=0 \
    -s INVOKE_RUN=0 \
    -s PRECISE_I32_MUL=0 \
    -s DISABLE_EXCEPTION_CATCHING=1 \
    -s EXPORTED_FUNCTIONS="${EXPORTED_FUNCTIONS}" \
    -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE="${LIBRARY_FUNCTIONS}" \
    -O2 \
    --pre-js pre.js \
    --post-js post.js \
    -o lib/libde265.js

echo "Compiling into asm-js (minimized)..."
emcc libde265-${LIBDE265_VERSION}/libde265/.libs/libde265.so \
    -s NO_EXIT_RUNTIME=1 \
    -s TOTAL_MEMORY=${TOTAL_MEMORY} \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=0 \
    -s INVOKE_RUN=0 \
    -s PRECISE_I32_MUL=0 \
    -s DISABLE_EXCEPTION_CATCHING=1 \
    -s EXPORTED_FUNCTIONS="${EXPORTED_FUNCTIONS}" \
    -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE="${LIBRARY_FUNCTIONS}" \
    -O3 \
    --pre-js pre.js \
    --post-js post.js \
    -o lib/libde265.min.js \
    -s CLOSURE_ANNOTATIONS=1 \
    --closure 1