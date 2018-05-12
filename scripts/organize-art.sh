#!/bin/bash

for art in bin/data/out-*.png
do
    day=`echo $art | perl -ne 'm/out-(\d+-\d+-\d+)-/ && print $1'`
    art_dir=art/images/$day
    mkdir -p $art_dir
    mv $art $art_dir
done

for art in bin/data/vid-out-*.mov
do
    day=`echo $art | perl -ne 'm/vid-out-(\d+-\d+-\d+)-/ && print $1'`
    art_dir=art/videos/$day
    mkdir -p $art_dir
    mv $art $art_dir
done

for art_dir in art/images/*
do
    ls $art_dir/*.png | perl -lne 'print "![](/$_?raw=true)\n"' > $art_dir/README.md
done
