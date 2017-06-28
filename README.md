# Texture-Atlas-Generator

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/1f48977e74514538bae8de37cb04d063)](https://www.codacy.com/app/kerndog73/Texture-Atlas-Generator?utm_source=github.com&utm_medium=referral&utm_content=Kerndog73/Texture-Atlas-Generator&utm_campaign=badger)

A command-line tool for packing spritesheets and a library for unpacking them

## What does it do?
This tool can be used to pack images into a spritesheet and produce an atlas describing the spritesheet. The job of the tool is described in a YAML based config file. You can see an [example of a config file](example%20config.yaml) to get an idea of what can be specified. Once you have a spritesheet and an atlas, you can use the unpacker to load the spritesheet and the atlas into you application.

### List of features

*  A tool for packing images from a folder into a spritesheet
   *  The images are identified in the atlas as the file names of the images without the extensions
   *  A separation can be specified between the sprites
   *  A white pixel can be specified. This is a single pixel or area of pixels on the spritesheet that are white
*  A library for reading the atlas
   *  A really nice C++ interface
   *  Loads both the atlas and the image

See [ROAD_MAP.md](ROAD_MAP.md) for a list of planned features.

## How do I build this on my machine?
1. Download the repo
2. Install homebrew `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
3. Install yaml-cpp `brew install yaml-cpp`
4. Open the Xcode project and build it (⌘ + B)
5. To check that the tool was installed correctly
   1. Go back to the terminal and type `packer`
6. To check that the library was installed correctly
   1. Open Finder
   2. Press (⌘ + ⇧ + G) and type `/usr/local/`
   3. In `lib` you should find `libUnpacker.dylib` and in `include/Unpacker` you should find `image.hpp` and `unpacker.hpp`

## How do I use the Unpacker

You can look in [image.hpp](Unpacker/image.hpp) and [unpacker.hpp](Unpacker/unpacker.hpp) for the relevent headers.

Here is a complete example for loading a spritesheet called `my images.png` and an atlas called `my images.atlas`.

    #include <Unpacker/unpacker.hpp>
    
    int main(int, const char **) {
      Spritesheet myImages;
      
      try {
        //Spritesheet is move only so this is a move-assign
        myImages = makeSpritesheet("my images.png", "my images.atlas");
      } catch (AtlasReadError &e) {
        std::cout << e.what() << '\n';
        return 1;
      }
      
      if (myImages.hasWhitepixel()) {
        const PosPx2 whitepx = myImages.getWhitepixel();
        std::cout << "Whitepixel is (" << whitepx.x << ", " << whitepx.y << ")\n";
      } else {
        //getWhitepixel will return Spritesheet::NO_WHITEPIXEL if there isn't one
        std::cout << "There is no whitepixel\n";
      }
      
      try {
        const RectPx mySprite = myImages.getSprite("my sprite");
        std::cout << "Position of \"my sprite\" is (" << mySprite.p.x << ", " << mySprite.p.y << ")\n";
        std::cout << "Size     of \"my sprite\" is (" << mySprite.s.x << ", " << mySprite.s.y << ")\n";
      } catch (SpriteNotFound &e) {
        std::cout << "There is no sprite called \"my sprite\"\n";
      }
      
      const Image &image = myImages.getImage();
      std::cout << "Size is (" << image.s.x << ", " << image.s.y << ")\n";
      
      //this is usually width * bytes per pixel
      std::cout << "Pitch is " << image.pitch << '\n';
      
      //this is usually 4
      std::cout << "Bytes per pixel is " << static_cast<SizePx>(image.format) << '\n';
      
      //data points to the top left pixel of the image (0, 0). 
      //Add pitch to the pointer to get (0, 1)
      std::cout << "Pointer to pixel data is " << image.data.get() << '\n';
      
      return 0;
    }

## How do I contribute?
Check out [CONTRIBUTING.md](CONTRIBUTING.md) to see what you can do. Then see if there's an item in the [ROAD_MAP.md](ROAD_MAP.md) that you can complete.
