#!/usr/bin/env sh

  cmake . && make -j8 && ./ngram ./input/pg10.txt "Ezra,1,1" "Malachi,4,5" "god"

