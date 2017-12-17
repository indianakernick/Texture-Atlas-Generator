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
#include <vector>
#include "image.hpp"
#include <string_view>
#include <unordered_map>

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
  
  using SpriteID = uint32_t;
  static constexpr SpriteID NULL_SPRITE = std::numeric_limits<SpriteID>::max();

  class Spritesheet {
  public:
    //only the factory function can make spritesheets
    friend Spritesheet makeSpritesheet(std::string_view, std::string_view);
    
    static const VecPx NO_WHITEPIXEL;
    
    Spritesheet();
    Spritesheet(const Spritesheet &) = delete;
    Spritesheet(Spritesheet &&) = default;
    ~Spritesheet() = default;
    
    Spritesheet &operator=(const Spritesheet &) = delete;
    Spritesheet &operator=(Spritesheet &&) = default;
    
    /// Returns NULL_SPRITE if the name is invalid
    SpriteID getIDfromName(std::string_view) const;
    
    bool hasWhitepixel() const;
    VecPx getWhitepixel() const;
    
    /// Throws std::out_of_range if SpriteID is invalid
    RectPx getSprite(SpriteID) const;
    /// Returns a reference to a member of this object
    const Image &getImage() const;
    
  private:
    explicit Spritesheet(Image &&);

    std::unordered_map<std::string, SpriteID> spriteNames;
    std::vector<RectPx> sprites;
    Image image;
    VecPx whitepixel;
  };

  Spritesheet makeSpritesheet(std::string_view atlas, std::string_view image);
}

#pragma GCC visibility pop

#endif
