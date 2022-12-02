# !/bin/bash

cd `dirname $0`
SOURCEROOT=`pwd`

build_option=''
if [ $# -gt 0 ]; then
    build_option=$1
fi

THIRD_PATH="$SOURCEROOT/third/"
THIRD_BUILD_DIR="/root/third/build/tengine/"
THIRD_INSTALL_DIR="/root/third/usr/local/"

mkdir -p $THIRD_BUILD_DIR
mkdir -p $THIRD_INSTALL_DIR

if [ ! -d "$THIRD_INSTALL_DIR/LuaJIT" ]; then
    pushd $THIRD_BUILD_DIR
    tar -xf $THIRD_PATH/LuaJIT-2.0.5.tar.gz -C .
    cd $THIRD_BUILD_DIR/LuaJIT-2.0.5
    make install PREFIX=$THIRD_INSTALL_DIR/LuaJIT
    cd -
    popd
fi
export LUAJIT_LIB=$THIRD_INSTALL_DIR/LuaJIT/lib
export LUAJIT_INC=$THIRD_INSTALL_DIR/LuaJIT/include/luajit-2.0

if [ ! -d "$THIRD_INSTALL_DIR/run" ]; then
    pushd $THIRD_BUILD_DIR
    tar -xf $THIRD_PATH/run.tgz -C .
    cd $THIRD_BUILD_DIR/run
    cmake3 -DCMAKE_INSTALL_PREFIX=$THIRD_INSTALL_DIR/run . && make install
    cd -
    popd
fi
export RUN_LIB=$THIRD_INSTALL_DIR/run/lib
export RUN_INC=$THIRD_INSTALL_DIR/run/include/

if [ $build_option == 'config' ]; then
    ./configure --prefix=/root/nginx \
      --with-pcre \
      --with-http_ssl_module \
      --with-http_v2_module \
      --with-http_gunzip_module \
      --with-http_gzip_static_module \
      --with-http_stub_status_module \
      --with-stream \
      --add-module=modules/ngx_http_lua_module/ \
      --add-module=modules/ngx_http_donkey_run/ \
      --with-ld-opt=-Wl,-rpath,$LUAJIT_LIB,-rpath,$RUN_LIB \
      --with-cc-opt=-O0
fi

make && make install
