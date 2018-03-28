#!/bin/bash

for art_dir in art/*
do
    ls $art_dir/*.png | perl -lne 'print "![](/$_?raw=true)\n"' > $art_dir/README.md
done
