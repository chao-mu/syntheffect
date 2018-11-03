#!/bin/bash
 # -ss 0.033 \
ffmpeg \
    -f rawvideo \
    -pixel_format rgb24 \
    -video_size $2 \
    -framerate 30 \
    -i $1 \
    -c:v libx264 \
    -preset slow \
    -crf 18 \
    -profile:v baseline \
    -level 3.0 \
    -movflags +faststart \
    -pix_fmt yuv420p \
    $3
