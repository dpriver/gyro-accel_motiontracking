#!/bin/bash


program=../build/bin/test/${1}

echo "loading program ${program}"

sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -C /etc/avrdude.conf -U flash:w:${program}

sudo putty -load arduino &> /dev/null
