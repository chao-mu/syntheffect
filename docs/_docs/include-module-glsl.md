---
title: include/module.glsl
permalink: /docs/include-module-glsl/
---

## Usage

At the verry begining of your shader module you sohuld include this file like so:

```c
#pragma include "../include/module.glsl"
```

(where the include path is relative and assumes that the shader is one directory down from the workspace root)

This module abstracts away the texture concept of GLSL and replaces them with input/output channels of individual
floats (values 0-1). This are the inputs and outputs that you connect to each other in your rack file.

## Uniforms/ins

```c
// Time in seconds
uniform float time;

// Whether this is the first frame
uniform bool firstPass;

// Texture coordinates
in vec2 textureCoordinate;
#define textureCoordinates textureCoordinate
```

## Constants

```
#define PI 3.14159265359
#define PI_HALF 1.5707963
#define HALF_PI 1.5707963
#define SQRT_2 1.41421356
```

## Functions

```c
// Get coordinates with value range of [-1, 1]
vec2 get_uv_1to1();

// Get coordinates with value range of [0, 1]
vec2 get_uv_0to1();

// Get coordinates in polar coordinates
vec2 get_uv_polar();

// translate coordinates in range -1 to 1 to texture coordinates.
vec2 from_uv_1to1(vec2 uv);

// translate coordinates in range 0 to 1 to texture coordinates
vec2 from_uv_0to1(vec2 uv);

// Map a value from one range to another
float map(float value, float min1, float max1, float min2, float max2);

// Return whether a float should be considered "true"
bool is_true(float v);

```

## Macros

### DEFINE\_INPUT(name, def, desc)

`DEFINE_INPUT` defines an input channel. It accepts three parameters: the name, the default value, and a description wrapped in the `DESC` macro.

Example:

```
DEFINE_INPUT(red, 0, DESC("The red component"))
```

This would in turn define the following functions

```c
# Input channel value ([0, 1]) at texture coordinate coord.
float input_red(vec2 coord) {...}

# Input the channel value ([0, 1]) at current texture coordinate.
float input_red() {...}

# True if a value was specified for this channel
bool passed_red() {...}
```

### DEFINE\_INPUT\_GROUP(name, in1, in2, in3)

`DEFINE_INPUT_GROUP` creates a pseudo-channel that's an aggregate (vec3) of three channels.

Example:

```c
DEFINE_INPUT(red, 0, DESC("The red component"))
DEFINE_INPUT(green, 0, DESC("The green component"))
DEFINE_INPUT(blue, 0, DESC("The blue component"))

DEFINE_INPUT_GROUP(rgb, red, green, blue)
```

This would in turn define the following functions, their uses the same as those defined by `DEFINE_INPUT`.

```c
vec3 input_rgb(vec2 coord) { ... }
vec3 input_rgb() { ... }
```

Note: you can not currently access your input groups in your rack file.

### DEFINE\_OUTPUT\_[1-18]\(name, desc)

`DEFINE_OUTPUT_1` (along with \_2, \_3, ... \_18) declares values you will be outputing from the module.


Example:

```c
DEFINE_OUTPUT_1(red, DESC("Red component"))
```

This would in turn define the following functions

```c
// Set this channel's value to be x
void output_red(float x) {...}

// Retrieve the last output of this channel
float last_output_red() {...}

// Retrieve the last output of this channel at these texture coordinates
float last_output_red(vec2 coords)
```

### DEFINE\_OUTPUT\_GROUP(name, out1, out2, out3)

`DEFINE_OUTPUT_GROUP` groups outputs, allowing you to treat them as one vector

```c
DEFINE_OUTPUT(red, DESC("Red component"))
```

Example:

```c
DEFINE_OUTPUT_1(red, DESC("The red component"))
DEFINE_OUTPUT_2(green, DESC("The green component"))
DEFINE_OUTPUT_3(blue, DESC("The blue component"))

DEFINE_OUTPUT_GROUP(rgb, red, green, blue)
```

This would in turn define the following functions, their uses the same as those defined by `DEFINE_OUTPUT`.

```c
void output_rgb(vec3 x) {...}
vec3 last_output_red() {...}
vec3 last_output_red(vec2 coords)
```
