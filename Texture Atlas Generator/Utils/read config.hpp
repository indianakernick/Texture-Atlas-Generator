//
//  read config.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_read_config_hpp
#define utils_read_config_hpp

#include "vector.hpp"
#include <yaml-cpp/yaml.h>

template <typename ValueType>
ValueType getOptional(const YAML::Node &node, const ValueType &def) {
  if (node) {
    return node.as<ValueType>();
  } else {
    return def;
  }
}

class AsVectorError final : YAML::ParserException {
public:
  explicit AsVectorError(const YAML::Mark &mark)
    : YAML::ParserException(mark, "Failed to interpret array as vec2") {}
};

template <typename ScalarType>
tvec2<ScalarType> getOptionalVector(const YAML::Node &node, const tvec2<ScalarType> def) {
  if (node.IsScalar()) {
    const ScalarType scalar = node.as<ScalarType>();
    return {scalar, scalar};
  } else if (node.IsSequence()) {
    if (node.size() != 2) {
      throw AsVectorError(node.Mark());
    }
    return {
      node[0].as<ScalarType>(),
      node[1].as<ScalarType>()
    };
  } else if (!node.IsDefined()) {
    return def;
  } else {
    throw AsVectorError(node.Mark());
  }
}

#endif
