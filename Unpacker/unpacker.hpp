//
//  unpacker.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 27/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef unpacker_unpacker_hpp
#define unpacker_unpacker_hpp

#include <string>
#include "image.hpp"
#include <unordered_map>
#include <experimental/string_view>

#pragma GCC visibility push(default)

namespace Unpack {
  class SpriteNotFound final : public std::range_error {
  public:
    SpriteNotFound();
  };

  class AtlasReadError final : public std::runtime_error {
  public:
    explicit AtlasReadError(const char *);
    explicit AtlasReadError(const std::exception &);
  };

  class Spritesheet {
  public:
    //only the factory function can make spritesheets
    friend Spritesheet makeSpritesheet(std::experimental::string_view, std::experimental::string_view);
    
    static const VecPx NO_WHITEPIXEL;
    
    Spritesheet();
    Spritesheet(const Spritesheet &) = delete;
    Spritesheet(Spritesheet &&) = default;
    ~Spritesheet() = default;
    
    Spritesheet &operator=(const Spritesheet &) = delete;
    Spritesheet &operator=(Spritesheet &&) = default;
    
    bool hasWhitepixel() const;
    VecPx getWhitepixel() const;
    RectPx getSprite(std::experimental::string_view) const;
    RectPx getSprite(const std::string &) const;
    const Image &getImage() const;
    
  private:
    explicit Spritesheet(Image &&);

    std::unordered_map<std::string, RectPx> sprites;
    Image image;
    VecPx whitepixel;
  };

  Spritesheet makeSpritesheet(std::experimental::string_view atlas, std::experimental::string_view image);
}

#pragma GCC visibility pop

#endif
