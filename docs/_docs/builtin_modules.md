---
title: Builtin Modules
permalink: /docs/builtin_modules/
---

## Summary

builtin modules have inputs that are set outside of the "inputs" attribute, unlike shader modules. These
can not change during runtime and consist of values such as file paths.

|Type|Name|Description|Author|
|----|----|-----------|------|
|[builtin/webcam](#webcam)|Webcam|Red RGB values from webcam|Danimalia Hackpoetica|
|[builtin/video](#video-player)|Video Player|Red RGB values from video|Danimalia Hackpoetica|
|[builtin/global](#global)|Global|Access global values and configure settings|Danimalia Hackpoetica|
|[builtin/audio](#audio-analyzer)|Audio Analyzer|Analyze audio file, useful for audio reactive racks|Danimalia Hackpoetica|
|[builtin/image](#image)|Image|Read an image file|Danimalia Hackpoetica|

## Details

### Webcam

Reads from your webcam device.

Type: builtin/webcam

Outputs:
* red - red component of pixel
* green - green component of pixel
* blue - blue component of pixel

### Video Player

Plays a video file (muted) and provides access to pixels

Type: builtin/video

Top level attributes:
* path - the relative (to the rack file) or absolute path to the video file

Outputs:
* red - red component of pixel
* green - green component of pixel
* blue - blue component of pixel

### Global

Configures resolution settings and provides access to time elapsed

Type: builtin/global

Top level attributes:
* width - Width of synthesizer output
* height - Height of synthesizer output

Outputs:
* time - Time elapsed in seconds

### Audio Analyzer

Plays and analyzes an audio file, providing frequency analysis across several bands.
The output can be magnified via the inputs. They do not effect the sound being played.

Type: builtin/audio

Top level attributes:
* path - The path to the audio file, relative (to the rack file) or absolute.

Inputs:
* 15khzGain - Multiples the 15khz band inorder to increase the signal
* 7-5khzGain - Multiples the 7.5khz band inorder to increase the signal
* 3khzGain - Multiples the 3khz band inorder to increase the signal
* 1-3khzGain - Multiples the 1.3khz band inorder to increase the signal
* 600hzGain - Multiples the 600hz band inorder to increase the signal
* 300hzGain - Multiples the 300hz band inorder to increase the signal
* 120hzGain - Multiples the 120hz band inorder to increase the signal

Outputs:
* 15khz - The current strength of the 15khz band
* 7-5khz - The current strength of the 7.5khz band
* 3khz - The current strength of the 3khz band
* 1-3khz -The current strength of the 1.3khz band
* 600hz - The current strength of the 600hz band
* 300hz - The current strength of the 300hz band
* 120hz - The current strength of the 120hz band

### Image

Read an image file.

Type: builtin/image

Top level attributes:
* path - the relative (to the rack file) or absolute path to the image file

Outputs:
* red - red component of pixel
* green - green component of pixel
* blue - blue component of pixel
