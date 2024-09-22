#!/bin/bash

BASE_DIR=~/QueryHouse-Framework

sudo apt-get update && \
sudo apt-get -y install make cmake build-essential vim sudo git \
    clang libmysqlclient-dev ninja-build pkg-config clang-format \
    libpq-dev libyaml-cpp-dev lld llvm bison python3-fire apt-transport-https \
    curl software-properties-common gnutls-dev ltrace


if [ ! -d "$BASE_DIR/bld" ]; then
    mkdir -p $BASE_DIR/bld
fi

cd $BASE_DIR/bld
cmake $BASE_DIR/mysql/ -DDOWNLOAD_BOOST=1 -DWITH_BOOST=../boost 
make -j

sudo cmake --install . --prefix /usr/local/mysql/

sudo chown $USER:$USER /usr/local/mysql/ -R
cd /usr/local/mysql/
if [ ! -d "mysql-files" ]; then
    mkdir mysql-files
    chmod 750 mysql-files
fi

bin/mysqld --initialize-insecure --user=$USER

cd $BASE_DIR
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -Wno-dev -DMYSQL=ON
cmake --build build -j