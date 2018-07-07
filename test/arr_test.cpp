#include <type_traits>

#include <gtest/gtest.h>
#include <rose/array.hpp>
#include <rose/slice.hpp>

using rose::Arr;
using rose::Slice;
using rose::MutSlice;

TEST(Arr, IsBothTrivialAndStandardLayout) {
  bool standard_layout = std::is_standard_layout<Arr<int, 1024>>::value;
  EXPECT_TRUE(standard_layout);
  bool trivial = std::is_trivial<Arr<int, 1024>>::value;
  EXPECT_TRUE(trivial);
  bool trivially_copyable = std::is_trivially_copyable<Arr<int, 1024>>::value;
  EXPECT_TRUE(trivially_copyable);
}

TEST(Arr, CanBeAggregateInitialized) {
  Arr<int, 5> arr = {1, 2, 3};
  EXPECT_TRUE(arr.arr[0] == 1);
  EXPECT_TRUE(arr.arr[1] == 2);
  EXPECT_TRUE(arr.arr[2] == 3);
  EXPECT_TRUE(arr.arr[3] == 0);
  EXPECT_TRUE(arr.arr[4] == 0);
}

TEST(Arr, get) {
  Arr<int, 3> arr = {1, 2, 3};
  EXPECT_TRUE(arr.get(0) == 1);
  EXPECT_TRUE(arr.get(1) == 2);
  EXPECT_TRUE(arr.get(2) == 3);
  EXPECT_TRUE(arr[0] == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr[2] == 3);

  // Abort: out of bounds
  // EXPECT_TRUE(arr[3] == 0);
}

TEST(Arr, last) {
  Arr<int, 3> arr = {1, 2, 3};
  EXPECT_TRUE(arr.last() == 3);
  Arr<int, 5> arr2 = {5, 4, 3, 2, 1};
  EXPECT_TRUE(arr2.last() == 1);

  // Abort: Not allowed to get last element of an empty array
  // Arr<int, 0> empty_arr = {};
  // empty_arr.last();
}

TEST(Arr, AssignValues) {
  Arr<int, 3> arr = {1, 2, 3};
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

TEST(Arr, len) {
  Arr<int, 5> arr = {1, 2, 3};
  EXPECT_TRUE(arr.len() == 5);
  auto arr2 = Arr<int, 5>();
  EXPECT_TRUE(arr2.len() == 5);
  auto arr3 = Arr<int, 1024>();
  EXPECT_TRUE(arr3.len() == 1024);
}

TEST(Arr, empty) {
  Arr<int, 3> arr = {1, 2, 3};
  EXPECT_TRUE(!arr.empty());
  Arr<int, 0> arr2 = {};
  EXPECT_TRUE(arr2.empty());
}

TEST(Arr, ptr) {
  Arr<int, 5> arr = {1, 2, 3};
  EXPECT_TRUE(arr.ptr()[0] == 1);
  EXPECT_TRUE(arr.ptr()[1] == 2);
  EXPECT_TRUE(arr.ptr()[2] == 3);
  arr.ptr()[1] = 4;
  EXPECT_TRUE(arr[1] == 4);
}

TEST(Arr, slice) {
  Arr<int, 6> arr = {1, 2, 3, 4, 5, 6};
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

TEST(Arr, mut_slice) {
  Arr<int, 6> arr = {1, 2, 3, 4, 5, 6};
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

TEST(Arr, MethodsCanBeCalledForConst) {
  const Arr<int, 3> arr = {1, 2, 3};
  EXPECT_TRUE(arr.get(0) == 1);
  EXPECT_TRUE(arr[1] == 2);
  EXPECT_TRUE(arr.ptr()[2] == 3);
  EXPECT_TRUE(arr.last() == 3);
  EXPECT_TRUE(arr.len() == 3);
  EXPECT_TRUE(!arr.empty());
  arr.slice(0, 1);
  arr.slice_from(1);
  arr.slice_to(1);
}
