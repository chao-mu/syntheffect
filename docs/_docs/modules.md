---
title: Modules
permalink: /docs/modules/
---

## Shader Modules

|Name|Type|Description|Author|
|----|----|-----------|------|
|[shaders/logo](#logo)|Logo|The author's tattoo and SynthEffect's logo|Danimalia Hackpoetica|
|[shaders/transform](#transform)|Transform|Apply simple transformations of pixel placement|Danimalia Hackpoetica|
|[shaders/hsv_mapper](#hsv-mapper)|HSV Mapper|Input HSV, output RGB and YCbCr|Danimalia Hackpoetica|
|[shaders/rgb_mapper](#rgb-mapper)|RGB Mapper|Map RGB to HSV and YCbCr|Danimalia Hackpoetica|
|[shaders/compositor](#compositor)|Compositor|Mix two sets of 3 channels according to a key channel|Danimalia Hackpoetica|
|[shaders/artifacts](#luma-blood)|Luma Blood|Simulate the luma signal bleeding into the chroma signals|Danimalia Hackpoetica|
|[shaders/time_tunnel](#time-tunnel)|Time Tunnel|Dr Who like graphics generator|Danimalia Hackpoetica|
|[shaders/pixelate](#pixelate)|Pixelate|Make look like pixels|Danimalia Hackpoetica|
|[shaders/dac](#digital-to-analog)|Digital to Analog|The opposite of the Analog to Digital module|Danimalia Hackpoetica|
|[shaders/magnitude](#magnitude)|Magnitude|The magnitude of either the first x1, x2, x3, or the difference between the magnitudes of x1-3 and y1-3|Danimalia Hackpoetica|
|[shaders/ramp](#simple-ramp)|Simple Ramp|Simple range horizontally and vertically|Danimalia Hackpoetica|
|[shaders/warpedo](#warpedo)|Warpedo|Warp the inputs|Danimalia Hackpoetica|
|[shaders/decay](#decay)|Decay|For each channel, mix in previous frame if its value is higher|Danimalia Hackpoetica|
|[shaders/adc](#analog-to-digital-conversion)|Analog to digital conversion|Represent input value in binary. Each output channel is 1 or 0|Danimalia Hackpoetica|
|[shaders/wind](#winder)|Winder|Wind the input in a circle|Danimalia Hackpoetica|
|[shaders/feedback](#feedback)|Feedback|Mix in previous frame to current frame, causing a echo like effect|Danimalia Hackpoetica|
|[shaders/osc](#oscilator)|Oscilator|Generate waves|Danimalia Hackpoetica|
|[shaders/edge_detector](#edge-detector)|Edge Detector|Run the channels through an edge detection algorithm (Scharr)|Danimalia Hackpoetica|
|[shaders/mirror_feedback](#mirror-feedback)|Mirror Feedback|Feedback simulating holding a mirror up to the camera, sorta|Danimalia Hackpoetica|
|[shaders/displace](#displace)|Displace|According to the displace channels, displace each pixel|Danimalia Hackpoetica|
|[shaders/change](#change)|Change|Measure the change between each frame.|Danimalia Hackpoetica|
|[shaders/color_encoder](#color-encoder)|Color Encoder|Tweak colors|Danimalia Hackpoetica|


### Logo

The author's tattoo and SynthEffect's logo

Inputs:

Outputs:
* *value* - the logo

### Transform

Apply simple transformations of pixel placement

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *mirrorX* - Mirror x axis
* *mirrorY* - Mirror y axis

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### HSV Mapper

Input HSV, output RGB and YCbCr

Inputs:
* *hue* - hue component
* *saturation* - saturation component
* *value* - value component

Outputs:
* *red* - red component
* *green* - green component
* *blue* - blue component
* *luma* - luma component
* *cb* - blue contribution to luma
* *cr* - red contribution to luma

### RGB Mapper

Map RGB to HSV and YCbCr

Inputs:
* *red* - red component
* *green* - green component
* *blue* - blue component

Outputs:
* *luma* - luma component
* *cb* - blue contribution to luma
* *cr* - red contribution to luma
* *hue* - hue component
* *saturation* - saturation component
* *value* - value component

### Compositor

Mix two sets of 3 channels according to a key channel

Inputs:
* *red* - red component
* *green* - green component
* *blue* - blue component
* *key* - whether left or right texture is used
* *keyThreshold* - the number dictating what 'key' value keys left/right
* *keyNegate* - If above 0.5, negates the keying
* *keyMixMode* - If above 0.5, mixes instead of keys
* *red2* - red component
* *green2* - green component
* *blue2* - blue component

Outputs:
* *red* - red component
* *green* - green component
* *blue* - blue component

### Luma Blood

Simulate the luma signal bleeding into the chroma signals

Inputs:
* *red* - red component
* *green* - green component
* *blue* - blue component
* *blood* - how much bleed yo want

Outputs:
* *red* - red component
* *green* - green component
* *blue* - blue component

### Time Tunnel

Dr Who like graphics generator

Inputs:
* *speed* - the speed of travel through the tunnel
* *travelOffset* - offset of travel time
* *seed* - offset of travel time

Outputs:
* *value* - value of the tunnel at a location

### Pixelate

Make look like pixels

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *amountX* - Amount chunked along X
* *amountY* - Amount chunked along Y

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Digital to Analog

The opposite of the Analog to Digital module

Inputs:
* *in1* - input channel for bit 1
* *in2* - input channel for bit 2
* *in3* - input channel for bit 3

Outputs:
* *value* - Resulting composed value

### Magnitude

The magnitude of either the first x1, x2, x3, or the difference between the magnitudes of x1-3 and y1-3

Inputs:
* *x1* - first component of vector
* *x2* - second component of vector
* *x3* - third component of vector
* *y1* - first component of vector
* *y2* - second component of vector
* *y3* - third component of vector

Outputs:
* *value* - Resulting magnitude of x
* *value_diff* - Resulting magnitude of x - y

### Simple Ramp

Simple range horizontally and vertically

Inputs:

Outputs:
* *h* - horizontal ramp
* *v* - vertical ramp

### Warpedo

Warp the inputs

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *amountX* - amount of indent
* *amountY* - amount of indent

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Decay

For each channel, mix in previous frame if its value is higher

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *amount* - amount the previous frame will be mixed in with the current

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Analog to digital conversion

Represent input value in binary. Each output channel is 1 or 0

Inputs:
* *value* - input signal to be classified
* *invert* - 0 is non inverted, 1 is inverted
* *bias* - Adjustment to input signal

Outputs:
* *out1* - output channel for bit 1
* *out2* - output channel for bit 2
* *out3* - output channel for bit 3

### Winder

Wind the input in a circle

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *amount* - amount of winding

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Feedback

Mix in previous frame to current frame, causing a echo like effect

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *amount* - amount the previous frame will be mixed in with the current

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Oscilator

Generate waves

Inputs:
* *freq* - frequency of wave
* *freqMod* - frequency modulation of wave
* *amplitude* - ampply resulting wave
* *shift* - offset of wave
* *vertical* - vertical or horizontal
* *phase* - phase of wave

Outputs:
* *sine* - sine wave
* *cosine* - cosine wave
* *sawtooth* - sawtooth wave
* *arcsine* - anti sine wave

### Edge Detector

Run the channels through an edge detection algorithm (Scharr)

Inputs:
* *red* - red component
* *green* - green component
* *blue* - blue component
* *negate* - negate the result

Outputs:
* *red* - red component
* *green* - green component
* *blue* - blue component

### Mirror Feedback

Feedback simulating holding a mirror up to the camera, sorta

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *rotate* - how much to rotate each reflection
* *factor* - amount to zoom (lower value, more zoom frames)
* *centerX* - center on x coordinate, -1 to 1
* *centerY* - center on y coordinate, -1 to 1

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Displace

According to the displace channels, displace each pixel

Inputs:
* *in1* - input channel 1
* *in2* - input channel 2
* *in3* - input channel 3
* *displaceH* - displacement signal for horizontal displacement, value is 0.5 - X 
* *displaceV* - displacement signal for vertical displacement, value is 0.5 - X 
* *displaceGain* - multiplies displaceH and displaceV

Outputs:
* *out1* - output channel for in1
* *out2* - output channel for in2
* *out3* - output channel for in3

### Change

Measure the change between each frame.

Inputs:
* *in1* - first component of vector
* *in2* - second component of vector
* *in3* - third component of vector
* *new* - whether there is new input

Outputs:
* *value* - Resulting magnitude of difference
* *in1_* - first component of vector
* *in2_* - second component of vector
* *in3_* - third component of vector

### Color Encoder

Tweak colors

Inputs:
* *red* - red component
* *redInvert* - red component, at 0 non-inverted, at 1 inverted
* *redBias* - red component bias
* *green* - green component
* *greenInvert* - green component, at 0 non-inverted, at 1 inverted
* *greenBias* - green component bias
* *blue* - blue component
* *blueInvert* - blue component, at 0 non-inverted, at 1 inverted
* *blueBias* - blue component bias

Outputs:
* *red* - red component
* *green* - green component
* *blue* - blue component

