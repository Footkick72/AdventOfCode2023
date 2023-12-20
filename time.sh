#!/bin/bash

for daydir in ./*; do
    if [ ! -d $daydir ]; then
        continue
    fi
    if [ $daydir = "./6" ]; then
        continue
    fi
    cd $daydir
    echo "day $daydir"
    for part in 1 2; do
        file=$part.cpp
        if [ -e ${part}-2.cpp ]; then
            file=${part}-2.cpp
        fi

        filename=$(basename $file)
        executable=${filename%.*}
        g++ -std=c++17 $file -o $executable -Ofast

        temp_file=$(mktemp)
        /usr/bin/time -o $temp_file ./$executable < in.txt > /dev/null
        user_time=$(grep user $temp_file | awk '{print $3}')
        rm $temp_file
        
        echo "$file: $user_time seconds"
    done
    echo ""
    cd ..
done

time bash ./time-all.sh