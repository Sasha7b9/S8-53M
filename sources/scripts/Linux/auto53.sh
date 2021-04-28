#!/usr/bin/env bash

while true
do

    git pull > out.txt
    FILENAME=out.txt
    FILESIZE=$(stat -c%s "$FILENAME")

    if [ $FILESIZE != "20" ]
    then

        time ./assembly.sh build all 1>S8-53M-good.build  2>S8-53M-fail.build
        FILENAME=S8-53M-fail.build
        FILESIZE=$(stat -c%s "$FILENAME")

        if [ $FILESIZE != "0" ]
        then
            ./send_fail.sh
        fi

        if [ $FILESIZE == "0" ]
        then
            ./send_good.sh

        fi
        rm S8-53M-fail.build
        rm S8-53M-good.build

    fi

    rm out.txt
    sleep 1

done
