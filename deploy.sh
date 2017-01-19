#! /bin/sh

home_app=~/app

home_zone=${home_app}/zone

./configure --prefix=${home_zone}

if test -d ${home_zone}; then
    rm -rf ${home_zone}
fi

make install

make distclean
