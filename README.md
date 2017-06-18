# Texture-Atlas-Generator
A command-line tool for packing spritesheets and creating bitmap fonts

## What does it do?
This tool can be used to pack images into a spritesheet and produce a YAML based atlas describing the spritesheet. This tool can also be used to create bitmap fonts by opening a TTF file, packing glyphs into an image and describing the glyphs in a YAML document. A full list of up-to-date options can be obtained by downloading and installing the executable and running it without arguments.

## Download the precompiled executable
Download the excecutable and put it in this directory `/usr/local/bin/`. Call `atlasgen` from the command-line (without arguments) to get the full list of options available.

## Or download the Xcode project
1. Download the project.
2. If you don't have homebrew then `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
3. Install freetype and yaml-cpp `brew install freetype2 && brew install yaml-cpp`
4. Install docopt. (not available with homebrew)
   * Download the [docopt repo](https://github.com/docopt/docopt.cpp). 
   * If you don't have `cmake` then install it with `brew install cmake`
   * Navigate to the repo with `cd`, `mkdir build && cd build && cmake -G Unix\ Makefiles ./../`
   * If that worked fine then `make && sudo make install`. You'll have to type in your admin password.
5. Open the Xcode project and build it.
6. To check that the tool was installed correctly, go back to the terminal and type `atlasgen`
