#!/bin/bash

for art_dir in art/*
do
    ls $art_dir/*.png | perl -lne 'print "![](/$_?raw=true)"' > $art_dir/README.md
done