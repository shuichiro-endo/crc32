#!/bin/bash

while read line
do
  ./crc32 "$line"
done < ./ntdll_func.txt
