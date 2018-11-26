---
title: Welcome
permalink: /docs/home/
redirect_from: /docs/index.html
---

## Getting started

<img class="d-inline-block" src="/img/linux-logo.png">
<img class="d-inline-block" src="/img/apple-logo.png">

<b>Warning: Alpha Software</b>

The general process of creating visuals with SynthEffect is to write a rack file in YAML and then launch the syntheffect executable. This rack file is a collection of modules you would like to use (some which you may have written), patched together to create pipelines of effects.  Examples of modules include video file input, shaders written in GLSL to apply effects, hardware controllers, and the "global" module used to specify project parameters such as width and height. You give each of these modules a name, a type, and a series of inputs that are either hard coded values or outputs of other modules.

All your work is conducted in subdirectories in the syntheffect-workspace, retrievable from github. This workspace contains all the modules that ship with syntheffect along with a projects directory and personal module directory for you to develop in.

### Installation

It is suggested you work on your projects within the syntheffect-workspace so start by cloning it from github

```
$ git clone https://github.com/chao-mu/syntheffect-workspace
$ cd syntheffect-workspace
```

To update the code base, whether it is a fork and you want upstream changes or if it is cloned directly, use update.sh

```
$ ./update.sh
```

Someday this will also pull in the latest SynthEffect binary, but for now you will need to [compile it](/docs/source) from source and add it to your PATH. See the [source compilation instructions](/docs/source) for more information.

### Workspace Layout

Once in the syntheffect-workspace directory, you will notice some directories

* `bin` - Executables, including the midi device onboarding script
* `examples` - Example rack files for various modules
* `include` - Reusable shader includes, including the omnipresent `module.glsl`
* `joysticks` - Modules representing various joysticks, like the xbox controller
* `midi` - Modules representing various midi controllers
* `personal` - Your personal modules you don't want to contribute
* `projects` - Your personal projects
* `shaders` - Shader modules

Your rack files will go in `projects/`, potentially along with assets (like videos and audio files) in whatever directory structure pleases you.

When you specify module types in your rack file, they get translated to paths within your workspace.

When you invoke syntheffect, you will need to specify the workspace directory with `-w` unless the workspace directory is your current directory (i.e. it defaults to `.`).

### Running your first patch

First let's run an existing patch to make sure all is well. Assuming your current directory is the workspace, try running the following:

```
$ syntheffect -p examples/shaders/adc/example-01.yml
```

You just ran the rack file `examples/shaders/adc/example-01.yml`. This file, written in YAML, specifies the flow of image data from one module to the next.

At the time of this writing, here is the source of the rack. Read the comments to learn how it works:

```yaml
##
# This example connects the brightness of each pixel in the webcam to the module ADC
# (analog digital converter), which in turn provides 3 outputs that act as bits
# describing the value numerically.
#
# adc provides the outputs out1, out2, and out3 so it can be used as an "out" module.
##

# Define a module called "external".
external:
    # Specify that it be of the type "builtin/webcam"
    module: builtin/webcam

# Define a second module called external_decoded
# This mapper provides coloring information. We are interested in the lumanince
external_decoded:
    # Specify that it be of the type "shaders/rgb_mapper"
    module: shaders/rgb_mapper
    # Provide the outputs of the webcam to this mapper
    inputs:
        red: external.red
        green: external.green
        blue: external.blue

# Define another module using the special name "out"
# Its output (if the outputs are out1, out2, out3 OR red, green, blue OR value)
# will be shown on your screen
out:
    # If you're curious about the code to this module, you can follow the path specified here
    # (an additional extension added according to the class of module).
    # In this case it's shaders/adc.frag
    module: shaders/adc
    inputs:
        value: external_decoded.luma
```

For information about each module and its inputs and outputs, see the appropiate documentation at the side.
