//
//  app.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <iostream>
#include <docopt/docopt.h>
#include "Font/user interface.hpp"
#include "Image/user interface.hpp"

static constexpr char USAGE[] = R"(Usage:
  atlasgen image [-i<path> -w<px> -s<px> -o<path>]
  atlasgen font -f<path> (-p<int>)... [-s<px> (-F<cp> -L<cp>) (-x<int> -y<int>) -o<path>]

Options:
  -f --font=path              Path to the font file
  -p --points=int             Point size of the font
  -w --whitepixel=px          Radius of the white pixel                         [default: -1]
  -s --sep=px                 The number of pixels separating each image        [default: 1]
  -i --input=path             Path to directory to find images                  [default: .]
  -o --output=path            Path to output file without an extension          [default: output]
  -x --dpi_x=int              Horizontal DPI                                    [default: 96]
  -y --dpi_y=int              Vertical DPI                                      [default: 96]
  -F --first=codepoint        ASCII Codepoint of the first character            [default: 32]
  -L --last=codepoint         ASCII Codepoint of the last character             [default: 126]
)";

ArgError::ArgError()
  : std::runtime_error("Invalid arguments") {}

using namespace std::literals;

InvalidArgVal::InvalidArgVal(const char *name)
  : std::runtime_error("Invalid value for argument "s + name) {}

void printBriefHelp() {
  std::cout << USAGE;
}

template <typename DST, typename SRC>
DST safeIntCast(const SRC src, const char *name) {
  if (std::numeric_limits<DST>::min() <= src && src <= std::numeric_limits<DST>::max()) {
    return static_cast<DST>(src);
  } else {
    throw InvalidArgVal(name);
  }
}

std::vector<FaceSize> getFaceSizes(std::map<std::string, docopt::value> &options) {
  const DPI dpi = {
    safeIntCast<SizePx>(options["--dpi_x"].asLong(), "dpi_x"),
    safeIntCast<SizePx>(options["--dpi_y"].asLong(), "dpi_y")
  };
  std::vector<FaceSize> sizes;
  const std::vector<std::string> &points = options["--points"].asStringList();
  for (auto p = points.cbegin(); p != points.cend(); p++) {
    SizePt pointSize;
    try {
      pointSize = safeIntCast<SizePt>(std::stol(*p), "points");
    } catch (...) {
      throw InvalidArgVal("points");
    }
    sizes.push_back({dpi, pointSize});
  }
  
  return sizes;
}

CodePointRange getCodepointRange(std::map<std::string, docopt::value> &options) {
  const CodePoint first = safeIntCast<CodePoint>(options["--first"].asLong(), "first");
  const CodePoint last = safeIntCast<CodePoint>(options["--last"].asLong(), "last");
  if (first > last) {
    throw InvalidArgVal("first and last");
  }
  return {
    first < ' ' ? ' ' : first,
    static_cast<CodePoint>(last == 127 ? 127 : last + 1)
  };
}

void runApp(const std::vector<std::string> &args) {
  if (args.size() == 1) {
    throw ArgError();
  }
  
  std::map<std::string, docopt::value> options;
  
  try {
    options = docopt::docopt_parse(USAGE, {args.cbegin() + 1, args.cend()});
  } catch (docopt::DocoptArgumentError &e) {
    std::cerr << "Argument error: " << e.what() << '\n';
    throw ArgError();
  } catch (docopt::DocoptLanguageError &e) {
    std::cerr << "Language Error: " << e.what() << '\n';
    throw std::runtime_error("");
  }
  
  /*for (const auto &o : options) {
    std::cout << o.first << ":  " << o.second << '\n';
  }*/
  
  if (options["image"].asBool()) {
    const long longWhitePixel = options["--whitepixel"].asLong();
    const SizePx whitePixel = longWhitePixel < 0
      ? NO_WHITE_PIXEL
      : safeIntCast<SizePx>(longWhitePixel, "whitepixel");
    createImageAtlas(
      options["--input"].asString(),
      options["--output"].asString(),
      whitePixel,
      safeIntCast<SizePx>(options["--sep"].asLong(), "sep")
    );
  } else {
    const std::vector<FaceSize> sizes = getFaceSizes(options);
    const CodePointRange range = getCodepointRange(options);
    createFontAtlas(
      options["--font"].asString(),
      options["--output"].asString(),
      sizes,
      range,
      safeIntCast<SizePx>(options["--sep"].asLong(), "sep")
    );
  }
}
