---
title: Source
permalink: /docs/source/
---

## Repositories

There are two repositories:

* [syntheffect](https://github.com/chao-mu/syntheffect) - the code that runs your rack file and loads modules from the workspace
* [syntheffect-workspace](https://github.com/chao-mu/syntheffect-workspace) - the workspace to house modules and to build your projects in

## Install from Source

The following directions are for Linux, but may be adapted by a MacOS hacker.

### Install Dependencies


First you will need to install git and other dependencies needed by SynthEffect.

#### Linux

Using your distributions package manager, perform the equivalent of one of the following:

```
sudo apt-get install git libtclap-dev libyaml-cpp-dev librtmidi
```

#### MacOS  (Brew)

```
brew install yaml-cpp rtmidi tclap
```

### Setup OpenFrameworks

To build the syntheffect binary from source you first need to setup an OpenFrameworks workspace. See https://openframeworks.cc/download/

All you have to do is download the source tarball, extract it, and run the `install_dependencies.sh` and `install_codecs.sh`
scripts in the appropriate supdirectory of `scripts/linux/`.


### Build project

Once you complete setting up OpenFrameworks (ensure to follow the setup guides closely), change directories into OpenFrameworks' subdirectory `apps/myApps`
and clone the syntheffect repository,

```
$ mkdir -p apps/myApps
$ git clone https://github.com/chao-mu/syntheffect
```

Next we need to download the addons that syntheffect requires
```
$ cd syntheffect
$ ./pull-addons.sh
```

Finally we need to build (add -j8 if you wantto build with 8 CPU threads, or substitute 8 for another number)

```
$ make
```

If you're on linux, the binary is now in `bin/syntheffect`. On Mac, it's elsewhere.

To see benchmarking output, compile as follows

```
$ CFLAGS=-DSYNTHEFFECT_BENCHMARK make
```

Next we suggest adding the bin directory to your PATH or create a symlink to it in the syntheffect-workspace.
