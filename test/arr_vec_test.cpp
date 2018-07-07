#include <type_traits>

#include <gtest/gtest.h>
#include <rose/array.hpp>
#include <rose/slice.hpp>

using rose::ArrVec;
using rose::Arr;
using rose::Slice;
using rose::MutSlice;

TEST(ArrVec, IsBothTrivialAndStandardLayout) {
  bool standard_layout = std::is_standard_layout<ArrVec<int, 1024>>::value;
  EXPECT_TRUE(standard_layout);
  bool trivial = std::is_trivial<ArrVec<int, 1024>>::value;
  EXPECT_TRUE(trivial);
  bool trivially_copyable = std::is_trivially_copyable<ArrVec<int, 1024>>::value;
  EXPECT_TRUE(trivially_copyable);
}

TEST(ArrVec, CanBeAggregateInitialized) {
  ArrVec<int, 5> arr = {3, {1, 2, 3}};
  EXPECT_TRUE(arr.arr[0] == 1);
  EXPECT_TRUE(arr.arr[1] == 2);
  EXPECT_TRUE(arr.arr[2] == 3);
}

TEST(ArrVec, push) {
  auto arr = ArrVec<int, 3>();
  arr.push(1337);
  EXPECT_TRUE(arr[0] == 1337);
  arr.push(42);
  arr.push(1234);
  EXPECT_TRUE(arr[1] == 42);
  EXPECT_TRUE(arr[2] == 1234);

  // Abort: full
  // arr.push(1);
}

TEST(ArrVec, pop) {
  auto arr = ArrVec<int, 16>();
  arr.push(1);
  arr.push(2);
  EXPECT_TRUE(arr.len() == 2);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  arr.pop();
  EXPECT_TRUE(arr.len() == 1);
  EXPECT_TRUE(arr[0] == 1);
  arr.pop();
  EXPECT_TRUE(arr.len() == 0);

  // Abort: empty
  // arr.pop();
}

TEST(ArrVec, insert) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  arr.push(3);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  arr.insert(1, 4);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 4);
  EXPECT_TRUE(arr[2] == 2);
  EXPECT_TRUE(arr[3] == 3);

  arr.insert(0, 0);
  // Abort: index out of bounds
  // arr.insert(5, 5);
  arr.insert(4, 5);
  EXPECT_TRUE(arr[0] == 0);
  EXPECT_TRUE(arr[1] == 1);
  EXPECT_TRUE(arr[2] == 4);
  EXPECT_TRUE(arr[3] == 2);
  EXPECT_TRUE(arr[4] == 5);
  EXPECT_TRUE(arr[5] == 3);

  // Abort: full
  // arr.insert(3, 6);
}

TEST(ArrVec, append_ptr) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  int other[] = {3, 4, 5};
  arr.append(other, 3);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  EXPECT_TRUE(arr[3] == 4);
  EXPECT_TRUE(arr[4] == 5);

  // Abort: Not enought space
  // int another[] = {6, 7};
  // arr.append(another, 2);
}

TEST(ArrVec, append_slice) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  int other[] = {3, 4, 5};
  Slice<int> slice = {3, other};
  arr.append(slice);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  EXPECT_TRUE(arr[3] == 4);
  EXPECT_TRUE(arr[4] == 5);

  // Abort: Not enought space
  // int another[] = {6, 7};
  // Slice<int> another_slice = {2, another};
  // arr.append(another_slice);
}

TEST(ArrVec, append_mut_slice) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  int other[] = {3, 4, 5};
  MutSlice<int> slice = {3, other};
  arr.append(slice);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  EXPECT_TRUE(arr[3] == 4);
  EXPECT_TRUE(arr[4] == 5);

  // Abort: Not enought space
  // int another[] = {6, 7};
  // MutSlice<int> another_slice = {2, another};
  // arr.append(another_slice);
}

TEST(ArrVec, append_arr) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  Arr<int, 3> other = {3, 4, 5};
  arr.append(other);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  EXPECT_TRUE(arr[3] == 4);
  EXPECT_TRUE(arr[4] == 5);

  // Abort: Not enought space
  // Arr<int, 2> another = {6, 7};
  // arr.append(another);
}

TEST(ArrVec, append_arr_vec) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  ArrVec<int, 16> other = {3, {3, 4, 5}};
  arr.append(other);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  EXPECT_TRUE(arr[3] == 4);
  EXPECT_TRUE(arr[4] == 5);

  // Abort: Not enought space
  // ArrVec<int, 16> another = {2, {6, 7}};
  // arr.append(another);
}

TEST(ArrVec, remove) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  arr.push(3);
  arr.push(4);
  arr.push(5);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  EXPECT_TRUE(arr[3] == 4);
  EXPECT_TRUE(arr[4] == 5);
  arr.remove(2);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 4);
  EXPECT_TRUE(arr[3] == 5);

  // Abort: index out of bounds
  // arr.remove(4);
}

TEST(ArrVec, clear) {
  auto arr = ArrVec<int, 6>();
  arr.push(1);
  arr.push(2);
  EXPECT_TRUE(arr.len() == 2);
  arr.clear();
  EXPECT_TRUE(arr.len() == 0);

  // Abort: Out of bounds
  // EXPECT_TRUE(arr[0] == 1);
}

TEST(ArrVec, get) {
  auto arr = ArrVec<int, 3>();
  arr.push(1);
  arr.push(2);
  arr.push(3);
  EXPECT_TRUE(arr.get(0) == 1);
  EXPECT_TRUE(arr.get(1) == 2);
  EXPECT_TRUE(arr.get(2) == 3);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);

  // Abort: out of bounds
  // EXPECT_TRUE(arr[3] == 0);
}

TEST(ArrVec, last) {
  ArrVec<int, 5> arr = {3, {1, 2, 3}};
  EXPECT_TRUE(arr.last() == 3);
  ArrVec<int, 5> arr2 = {5, {5, 4, 3, 2, 1}};
  EXPECT_TRUE(arr2.last() == 1);

  // Abort: Not allowed to get last element of an empty array
  // auto empty_arr = ArrVec<int, 5>();
  // empty_arr.last();
}

TEST(ArrVec, AssignValues) {
  auto arr = ArrVec<int, 16>();
  arr.push(1);
  arr.push(2);
  arr.push(3);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);
  arr[0] = 2;
  EXPECT_TRUE(arr[0] == 2);
  arr.get(1) = 4;
  EXPECT_TRUE(arr[1] == 4);
  arr.last() = 7;
  EXPECT_TRUE(arr[2] == 7);
}

TEST(ArrVec, len) {
  auto arr = ArrVec<int, 16>();
  EXPECT_TRUE(arr.len() == 0);
  arr.push(1337);
  EXPECT_TRUE(arr.len() == 1);
  arr.push(1337);
  EXPECT_TRUE(arr.len() == 2);
  arr.pop();
  EXPECT_TRUE(arr.len() == 1);
}

TEST(ArrVec, empty) {
  auto arr = ArrVec<int, 16>();
  EXPECT_TRUE(arr.empty());
  arr.push(1337);
  EXPECT_TRUE(!arr.empty());
  arr.pop();
  EXPECT_TRUE(arr.empty());
}

TEST(ArrVec, full) {
  auto arr = ArrVec<int, 2>();
  arr.push(1);
  EXPECT_TRUE(!arr.full());
  arr.push(2);
  EXPECT_TRUE(arr.full());
  arr.pop();
  EXPECT_TRUE(!arr.full());
}

TEST(ArrVec, capacity) {
  auto arr = ArrVec<int, 8>();
  arr.push(1);
  arr.push(2);
  EXPECT_TRUE(arr.capacity() == 8);
  auto arr2 = ArrVec<int, 2>();
  EXPECT_TRUE(arr2.capacity() == 2);
}

TEST(ArrVec, ptr) {
  ArrVec<int, 5> arr = {3, {1, 2, 3}};
  EXPECT_TRUE(arr.ptr()[0] == 1);
  EXPECT_TRUE(arr.ptr()[1] == 2);
  EXPECT_TRUE(arr.ptr()[2] == 3);
  arr.ptr()[1] = 4;
  EXPECT_TRUE(arr[1] == 4);
}

TEST(ArrVec, slice) {
  ArrVec<int, 16> arr = {6, {1, 2, 3, 4, 5, 6}};
  { // slice()
    Slice<int> slice = arr.slice();
    EXPECT_TRUE(slice.len() == arr.len());
    EXPECT_TRUE(slice.ptr() == arr.ptr());
  }
  { // slice(from, to)
    Slice<int> slice = arr.slice(2, 4);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 3);
    EXPECT_TRUE(slice[1] == 4);
  }
  { // slice_from()
    Slice<int> slice = arr.slice_from(4);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 5);
    EXPECT_TRUE(slice[1] == 6);
  }
  { // slice_to()
    Slice<int> slice = arr.slice_to(2);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 1);
    EXPECT_TRUE(slice[1] == 2);
  }
}

TEST(ArrVec, mut_slice) {
  ArrVec<int, 16> arr = {6, {1, 2, 3, 4, 5, 6}};
  { // mut_slice()
    MutSlice<int> slice = arr.mut_slice();
    EXPECT_TRUE(slice.len() == arr.len());
    EXPECT_TRUE(slice.ptr() == arr.ptr());
  }
  { // mut_slice(from, to)
    MutSlice<int> slice = arr.mut_slice(2, 4);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 3);
    EXPECT_TRUE(slice[1] == 4);
  }
  { // mut_slice_from()
    MutSlice<int> slice = arr.mut_slice_from(4);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 5);
    EXPECT_TRUE(slice[1] == 6);
  }
  { // mut_slice_to()
    MutSlice<int> slice = arr.mut_slice_to(2);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 1);
    EXPECT_TRUE(slice[1] == 2);
  }
}

TEST(ArrVec, MethodsCanBeCalledForConst) {
  const ArrVec<int, 5> arr = {3, {1, 2, 3}};
  EXPECT_TRUE(arr.get(0) == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr.ptr()[2] == 3);
  EXPECT_TRUE(arr.last() == 3);
  EXPECT_TRUE(arr.len() == 3);
  EXPECT_TRUE(!arr.empty());
  EXPECT_TRUE(arr.capacity() == 5);
  arr.slice(0, 1);
  arr.slice_from(1);
  arr.slice_to(1);
}
