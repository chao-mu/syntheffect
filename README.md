# SynthEffect

![Under Construction](http://www.textfiles.com/underconstruction/HeHeartlandPark2601underconstructionbar9.gif)

SynthEffect is my personal VJing software. It is a stack of OpenFrameworks and GLSL shaders meant to be controlled by video game joysticks and their USB clones. 

This software is currently for my personal use. If you'd like to try to get it going for yourself, file a ticket and I'll help.

![Under Construction](http://www.textfiles.com/underconstruction/vovooo13gifSupercompressed_koala-construct3.GIF)


## Architecture

Input, Assets, Effects, Renderer

       Input
       _ | _
     /      \
    [messages] 
    /        \
Assets      Effects
         

## Recording

### YouTube

```
ffmpeg -f rawvideo -pixel_format rgb24 -video_size 1280x720 -framerate 30 -i output.rgb24 -c:v libx264 -preset ultrafast -vf scale=iw*4:-1:flags=neighbor+bitexact+accurate_rnd+full_chroma_int+full_chroma_inp+print_info output.mp4
```

### Facebook

```
ffmpeg -f rawvideo -pixel_format rgb24 -video_size 1280x720 -framerate 30 -i output.rgb24 -c:v libx264 -crf 18 -preset slow -c:a copy output.mp4
```
