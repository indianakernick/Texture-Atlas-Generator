# Texture-Atlas-Generator

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/1f48977e74514538bae8de37cb04d063)](https://www.codacy.com/app/kerndog73/Texture-Atlas-Generator?utm_source=github.com&utm_medium=referral&utm_content=Kerndog73/Texture-Atlas-Generator&utm_campaign=badger)

A command-line tool for packing spritesheets and creating bitmap fonts

## What does it do?
This tool can be used to pack images into a spritesheet and produce an atlas describing the spritesheet. The job of the tool is described in a YAML based config file. You can see an [example of a config file](example%20config.yaml) to get an idea of what can be specified.

### List of features

*  Packing images from a folder into a spritesheet
*  The images are identified in the atlas as the file names of the images with the extensions
*  A separation can be specified between the sprites
*  A white pixel can be specified. This is a single pixel or area of pixels on the spritesheet that are white

See [ROAD_MAP.md](ROAD_MAP.md) for a list of planned features.

## How do I build this on my machine?
1. Download the project.
2. If you don't have homebrew then `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
3. Install freetype and yaml-cpp `brew install freetype2 && brew install yaml-cpp`
4. Open the Xcode project and build it (⌘ + B).
5. To check that the tool was installed correctly, go back to the terminal and type `atlasgen`

## How do I contribute?
Check out [CONTRIBUTING.md](CONTRIBUTING.md) to see what you can do. Then see if  there's an item in the [ROAD_MAP.md](ROAD_MAP.md) that you can complete.
