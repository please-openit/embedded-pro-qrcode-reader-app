#!/bin/bash
gpio mode 26 output
while : ; do
    read -N 10000000 -t 0.01
    read token
    httpcode=$(curl -s -o /dev/null -w "%{http_code}" https://access.please-open.it/access/ -H 'Authorization: $token' -H 'controllerId: <doorId>' )

    if [ $httpcode = 200 ]; then
        echo "access OK"
        play success.wav &
        gpio write 26 1
        sleep 2
        gpio write 26 0

    else
        echo "access denied"
        play error.wav &
    fi
done;
