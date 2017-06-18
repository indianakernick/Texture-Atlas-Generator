//
//  freetype raii.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 30/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_freetype_raii_hpp
#define font_freetype_raii_hpp

#include <utility>
#include <ft2build.h>
#include FT_TYPES_H

template <typename T, FT_Error (*DELETER) (T)>
class FTHandle {
public:
  FTHandle()
    : obj(nullptr) {}
  FTHandle(const FTHandle &) = delete;
  FTHandle(FTHandle &&other) {
    obj = std::exchange(other.obj, nullptr);
  }
  ~FTHandle() {
    DELETER(obj);
  }
  
  FTHandle &operator=(const FTHandle &) = delete;
  FTHandle &operator=(FTHandle &&other) {
    DELETER(obj);
    obj = std::exchange(other.obj, nullptr);
    return *this;
  }
  
  void swap(FTHandle &other) {
    std::swap(obj, other.obj);
  }
  
  void del() {
    DELETER(obj);
    obj = nullptr;
  }
  
  operator T() const {
    return obj;
  }
  
  explicit operator bool() const {
    return obj;
  }
  bool operator!() const {
    return !obj;
  }
  
  T operator->() {
    return obj;
  }
  const T operator->() const {
    return obj;
  }
  
  T *operator&() {
    return &obj;
  }
  const T *operator&() const {
    return &obj;
  }
  
  template <typename U>
  U as() {
    return reinterpret_cast<U>(obj);
  }
  template <typename U>
  const U as() const {
    return reinterpret_cast<const U>(obj);
  }
  
private:
  T obj;
};

#define FT_HANDLE(type) FTHandle<FT_##type, FT_Done_##type>

#endif
