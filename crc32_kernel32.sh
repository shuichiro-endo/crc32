#!/bin/bash

while read line
do
  ./crc32 "$line"
done < ./kernel32_func.txt
