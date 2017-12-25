#!/bin/bash

for entry in `find ../Demo ../Source -type f -name "*"`
do
  if [ -f "$entry" ]; then
    line=`file -i "$entry" | grep uft-16`
    #line=`file -i "$entry" | grep ascii`
    if [ ! -z "$line" ]; then
      echo "$entry"
      iconv -f UTF-16 -t ASCII -o $entry $entry
    fi
  fi
done
