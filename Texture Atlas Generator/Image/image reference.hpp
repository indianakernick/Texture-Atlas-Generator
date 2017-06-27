//
//  image reference.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 27/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_image_reference_hpp
#define image_image_reference_hpp

#include "image.hpp"
#include <stdexcept>

class BoundsError final : std::range_error {
public:
  BoundsError();
};

///Create an image that refers to an area on another image. The life of the
///source should be longer that the life of the reference to prevent dangling
///pointers.
Image imageRef(const Image &, RectPx);

///Create an image that refers to another image. The life of the
///source should be longer that the life of the reference to prevent dangling
///pointers.
Image imageRef(const Image &);

#endif
