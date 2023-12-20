#!/bin/bash

for daydir in ./*; do
    if [ ! -d $daydir ]; then
        continue
    fi
    if [ $daydir = "./6" ]; then
        continue
    fi
    cd $daydir
    for part in 1 2; do
        file=$part.cpp
        if [ -e ${part}-2.cpp ]; then
            file=${part}-2.cpp
        fi

        filename=$(basename $file)
        executable=${filename%.*}

        ./$executable < in.txt > /dev/null
    done
    cd ..
done