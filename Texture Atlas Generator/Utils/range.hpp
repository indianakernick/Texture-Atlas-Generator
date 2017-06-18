//
//  range.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 13/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_range_hpp
#define utils_range_hpp

#include <iterator>
#include <type_traits>

template <typename ITER, bool = std::is_integral<ITER>::value>
class Range;

///An iterator range
template <typename ITER>
class Range<ITER, false> {
public:
  using iterator = ITER;
  using difference_type = typename std::iterator_traits<ITER>::difference_type;
  using value_type = typename std::iterator_traits<ITER>::value_type;
  using pointer = typename std::iterator_traits<ITER>::pointer;
  using reference = typename std::iterator_traits<ITER>::reference;
  
private:
  static constexpr bool READ_ONLY = std::is_const<std::remove_reference_t<reference>>::value;
  using category = typename std::iterator_traits<ITER>::iterator_category;
  
  #define TAG_IS(tagname) \
    std::is_same<category, std::tagname##_iterator_tag>::value
  
  static constexpr bool RANDOM_ACCESS = TAG_IS(random_access);
  static constexpr bool BI_DIR = TAG_IS(bidirectional);
  static constexpr bool FORWARD = TAG_IS(forward);
  static constexpr bool INPUT = TAG_IS(input);
  static constexpr bool OUTPUT = TAG_IS(output);
  
  #undef TAG_IS
  
  static constexpr bool LEAST_RANDOM_ACCESS = RANDOM_ACCESS;
  static constexpr bool LEAST_BI_DIR = BI_DIR || LEAST_RANDOM_ACCESS;
  static constexpr bool LEAST_FORWARD = FORWARD || LEAST_BI_DIR;
  static constexpr bool LEAST_INPUT = INPUT || LEAST_FORWARD;
  static constexpr bool LEAST_OUTPUT = OUTPUT || !READ_ONLY;
  
  void checkValid() const {
    if (RANDOM_ACCESS && mBegin > mEnd) {
      throw std::range_error("Bad range");
    }
  }
  
public:
  Range() = delete;
  Range(const iterator begin, const iterator end)
    : mBegin(begin), mEnd(end) {
    checkValid();
  }

  void begin(const iterator newBegin) {
    mBegin = newBegin;
    checkValid();
  }
  iterator begin() const {
    return mBegin;
  }
  iterator cbegin() const {
    static_assert(READ_ONLY);
    return mBegin;
  }
  
  void end(const iterator newEnd) {
    mEnd = newEnd;
    checkValid();
  }
  iterator end() const {
    return mEnd;
  }
  iterator cend() const {
    static_assert(READ_ONLY);
    return mEnd;
  }
  
  reference front() {
    static_assert(LEAST_INPUT);
    return *mBegin;
  }
  
  reference back() {
    static_assert(LEAST_BI_DIR);
    if (RANDOM_ACCESS) {
      return *(mEnd - 1);
    } else if (BI_DIR) {
      return *(std::prev(mEnd));
    }
  }
  
  difference_type distance() const {
    return std::distance(mBegin, mEnd);
  }
  //we can only guarantee that distance will be positive if the
  //iterators are random access
  size_t size() const {
    static_assert(RANDOM_ACCESS);
    return static_cast<size_t>(mEnd - mBegin);
  }
  
  void size(const size_t newSize) {
    static_assert(RANDOM_ACCESS);
    mEnd = mBegin + newSize;
  }
  
  reference operator[](const difference_type i) {
    if (RANDOM_ACCESS) {
      return *(mBegin + i);
    } else {
      iterator iter = mBegin;
      std::advance(iter, i);
      return *(iter);
    }
  }
  
  reference at(const difference_type i) {
    if (RANDOM_ACCESS) {
      iterator iter = mBegin + i;
      if (mBegin <= iter && iter < mEnd) {
        return *iter;
      } else {
        throw std::range_error("Index out of range");
      }
    } else {
      iterator iter = mBegin;
      std::advance(iter, i);
      return *(iter);
    }
  }
  
private:
  iterator mBegin;
  iterator mEnd;
};

///An integer range
template <typename INT>
class Range<INT, true> {
public:
  using value_type = INT;
  using difference_type = std::make_signed_t<INT>;
  
  Range() = delete;
  Range(const value_type begin, const value_type end)
    : mBegin(begin), mEnd(end) {
    checkValid();
  }
  
  void begin(const value_type newBegin) {
    mBegin = newBegin;
    checkValid();
  }
  value_type begin() const {
    return mBegin;
  }
  
  void end(const value_type newEnd) {
    mEnd = newEnd;
    checkValid();
  }
  value_type end() const {
    return mEnd;
  }
  
  difference_type distance() const {
    return mEnd - mBegin;
  }
  size_t size() const {
    return static_cast<size_t>(mEnd - mBegin);
  }
private:
  value_type mBegin;
  value_type mEnd;
  
  void checkValid() const {
    if (mBegin > mEnd) {
      throw std::range_error("Bad range");
    }
  }
};

#endif
