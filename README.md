# Texture-Atlas-Generator

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/1f48977e74514538bae8de37cb04d063)](https://www.codacy.com/app/kerndog73/Texture-Atlas-Generator?utm_source=github.com&utm_medium=referral&utm_content=Kerndog73/Texture-Atlas-Generator&utm_campaign=badger)

A command-line tool for packing spritesheets and creating bitmap fonts

## What does it do?
This tool can be used to pack images into a spritesheet and produce a YAML based atlas describing the spritesheet. This tool can also be used to create bitmap fonts by opening a TTF file, packing glyphs into an image and describing the glyphs in a YAML document. A full list of up-to-date options can be found in [app.cpp](Texture%20Atlas%20Generator/app.cpp)

An example of what this tool can create is a bitmap font. Here's the ASCII range of Arial at 12pt and 24pt at 96DPI. The assosiated atlas contains the bearing, advance and size of each glyph as well as the kerning table.

![Arial 12pt and 24pt](https://i.imgur.com/Jj55BiP.png)

## How do I build this on my machine?
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

## How do I contribute?
Check out [CONTRIBUTING.md](CONTRIBUTING.md) to see what you can do. Then see if  there's an item in the [ROAD_MAP.md](ROAD_MAP.md) that you can complete.
