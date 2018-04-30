#!/bin/bash

for art in bin/data/out-*.png
do
    day=`echo $art | perl -ne 'm/out-(\d+-\d+-\d+)-/ && print $1'`
    art_dir=art/$day
    mkdir -p $art_dir
    mv $art $art_dir
done

for art_dir in art/*
do
    ls $art_dir/*.png | perl -lne 'print "![](/$_?raw=true)\n"' > $art_dir/README.md
done
