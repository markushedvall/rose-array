#ifndef ROSE_ARRAY_HPP
#define ROSE_ARRAY_HPP

#include <cassert>
#include <cstddef>
#include <cstring>

#include <rose/slice.hpp>

namespace rose {

  template<class TYPE, size_t CAPACITY>
  struct Arr {

    TYPE arr[CAPACITY];

    TYPE& get(size_t index) noexcept {
      assert(index < CAPACITY);
      return arr[index];
    }

    const TYPE& get(size_t index) const noexcept {
      assert(index < CAPACITY);
      return arr[index];
    }

    TYPE& operator[](size_t index) noexcept {
      return get(index);
    }

    const TYPE& operator[](size_t index) const noexcept {
      return get(index);
    }

    TYPE& last() noexcept {
      assert(!empty());
      return arr[CAPACITY - 1];
    }

    const TYPE& last() const noexcept {
      assert(!empty());
      return arr[CAPACITY - 1];
    }

    size_t len() const noexcept {
      return CAPACITY;
    }

    bool empty() const noexcept {
       return (CAPACITY == 0);
    }

    TYPE* ptr() noexcept {
      return arr;
    }

    const TYPE* ptr() const noexcept {
      return arr;
    }

    Slice<TYPE> slice() const noexcept {
      return {CAPACITY, arr};
    }

    Slice<TYPE> slice(size_t from, size_t to) const noexcept {;
      assert(from < to);
      assert(to <= CAPACITY);
      return {to - from, arr + from};
    }

    Slice<TYPE> slice_from(size_t from) const noexcept {
      assert(from < CAPACITY);
      return {CAPACITY - from, arr + from};
    }

    Slice<TYPE> slice_to(size_t to) const noexcept {
      assert(to <= CAPACITY);
      return {to, arr};
    }

    MutSlice<TYPE> mut_slice() noexcept {
      return {CAPACITY, arr};
    }

    MutSlice<TYPE> mut_slice(size_t from, size_t to) noexcept {;
      assert(from < to);
      assert(to <= CAPACITY);
      return {to - from, arr + from};
    }

    MutSlice<TYPE> mut_slice_from(size_t from) noexcept {
      assert(from < CAPACITY);
      return {CAPACITY - from, arr + from};
    }

    MutSlice<TYPE> mut_slice_to(size_t to) noexcept {
      assert(to <= CAPACITY);
      return {to, arr};
    }

  };


  template<class TYPE, size_t CAPACITY>
  struct ArrVec {

    size_t length;
    Arr<TYPE, CAPACITY> arr;

    void push(const TYPE& value) noexcept {
      assert(length < CAPACITY);
      arr[length] = value;
      length += 1;
    }

    void pop() noexcept {
      assert(length > 0);
      length -= 1;
    }

    void insert(size_t index, const TYPE& value) noexcept {
      assert(length < CAPACITY);
      assert(index < length);
      size_t bytes = (length - index) * sizeof(TYPE);
      std::memmove(&arr[index + 1], &arr[index], bytes);
      arr[index] = value;
      length += 1;
    }

    void append(const TYPE* values, size_t num) noexcept {
      assert((length + num) <= CAPACITY);
      std::memmove(&arr[length], values, num * sizeof(TYPE));
      length += num;
    }

    void append(const Slice<TYPE>& slice) noexcept {
      assert((length + slice.len()) <= CAPACITY);
      std::memmove(&arr[length], slice.ptr(), slice.len() * sizeof(TYPE));
      length += slice.len();
    }

    void append(const MutSlice<TYPE>& slice) noexcept {
      assert((length + slice.len()) <= CAPACITY);
      std::memmove(&arr[length], slice.ptr(), slice.len() * sizeof(TYPE));
      length += slice.len();
    }

    template<size_t OTHER_CAPACITY>
    void append(const Arr<TYPE, OTHER_CAPACITY>& other) noexcept {
      assert((length + other.len()) <= CAPACITY);
      std::memmove(&arr[length], other.ptr(), other.len() * sizeof(TYPE));
      length += other.len();
    }

    template<size_t OTHER_CAPACITY>
    void append(const ArrVec<TYPE, OTHER_CAPACITY>& other) noexcept {
      assert((length + other.length) <= CAPACITY);
      std::memmove(&arr[length], other.ptr(), other.len() * sizeof(TYPE));
      length += other.length;
    }

    void remove(size_t index) {
      assert(index < length);
      size_t bytes = (length - index) * sizeof(TYPE);
      std::memmove(&arr[index], &arr[index + 1], bytes);
      length -= 1;
    }

    void clear() noexcept {
      length = 0;
    }

    TYPE& get(size_t index) noexcept {
      assert(index < CAPACITY);
      assert(index < length);
      return arr[index];
    }

    const TYPE& get(size_t index) const noexcept {
      assert(index < CAPACITY);
      assert(index < length);
      return arr[index];
    }

    TYPE& operator[](size_t index) noexcept {
      return get(index);
    }

    const TYPE& operator[](size_t index) const noexcept {
      return get(index);
    }

    TYPE& last() noexcept {
      assert(!empty());
      return arr[length - 1];
    }

    const TYPE& last() const noexcept {
      assert(!empty());
      return arr[length - 1];
    }

    size_t len() const noexcept {
      return length;
    }

    bool empty() const noexcept {
      return length == 0;
    }

    bool full() const noexcept {
      return length == CAPACITY;
    }

    size_t capacity() const noexcept {
      return CAPACITY;
    }

    TYPE* ptr() noexcept {
      return arr.ptr();
    }

    const TYPE* ptr() const noexcept {
      return arr.ptr();
    }

    Slice<TYPE> slice() const noexcept {
      return {length, arr.ptr()};
    }

    Slice<TYPE> slice(size_t from, size_t to) const noexcept {;
      assert(from < to);
      assert(to <= length);
      return {to - from, arr.ptr() + from};
    }

    Slice<TYPE> slice_from(size_t from) const noexcept {
      assert(from < length);
      return {length - from, arr.ptr() + from};
    }

    Slice<TYPE> slice_to(size_t to) const noexcept {
      assert(to <= length);
      return {to, arr.ptr()};
    }

    MutSlice<TYPE> mut_slice() noexcept {
      return {length, arr.ptr()};
    }

    MutSlice<TYPE> mut_slice(size_t from, size_t to) noexcept {;
      assert(from < to);
      assert(to <= length);
      return {to - from, arr.ptr() + from};
    }

    MutSlice<TYPE> mut_slice_from(size_t from) noexcept {
      assert(from < length);
      return {length - from, arr.ptr() + from};
    }

    MutSlice<TYPE> mut_slice_to(size_t to) noexcept {
      assert(to <= length);
      return {to, arr.ptr()};
    }

  };

}

#endif
