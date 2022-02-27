#!/usr/bin/env sh

  cmake . && make -j8 && ./ngram ./input/pg10.txt "The First Book of Moses: Called Genesis,1,1" "Ezra,2,5" "hosts"

