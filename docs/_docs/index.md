---
title: Welcome
permalink: /docs/home/
redirect_from: /docs/index.html
---

## Getting started

The general process of creating visuals with SynthEffect is to write a rack file in YAML. This rack file is a collection of modules you would like to use, patched together to create pipelines of effects.  Examples of modules include video file input, shaders written in GLSL to apply effects, hardware controllers, and the "global" module used to specify project parameters such as width and height. You give each of these modules a name, a type, and a series of inputs that are either hard coded values or outputs of other modules.

Here is an example of making your webcam black and white.

```yaml
# Make an instance of a webcam module, naming it "exteneral"
external:
    # Specify the type
    module: "webcam"

# Make an instance of the rgb_mapper module (used to map rgb to HSV and YUV),
# naming it "mapper"
mapper:
    module: "rgb_mapper"
    # Define this module's inputs
    inputs:
        # Map the input red to the "red" output of our module "external" (could be any input, even green!)
        red: "external.red"
        green: "external.green"
        blue: "external.blue"

# The special name "out" is where SynthEffect gets the values displayed to the monitor,
# Any module that defines a red, green, and blue (internally as its first 3 outputs declared)
# can be used for output
out:
    module: "color_encoder"
    inputs:
        red: "mapper.luma"
        green: "mapper.luma"
        blue: "mapper.luma"
```

We could go a step futher and remap colors by using `color_encoder`'s redInvert, greenInvert, and blueInvert inputs


```yaml
out:
    module: "color_encoder"
    inputs:
        red: "mapper.luma"
        redInvert: 0.2
        green: "mapper.luma"
        greenInvert: 1
        blue: "mapper.luma"
        blueInvert: 0.5
```

And then perhaps use redBias, greenBias, and blueBias to tweak the colors.
