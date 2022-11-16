#include <algorithm>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "MyVector.hpp"

TEST_CASE("MyVector")
{
    const std::size_t SIZE = 8;
    MyVector<int> arr;
    for (int i = 0; i < SIZE; i++)
        arr.push_back(i);
    
    CHECK(std::equal(arr.begin(), arr.end(), MyVector<int>{0, 1, 2, 3, 4, 5, 6, 7}.begin()));
    CHECK(arr.size() == SIZE);
    CHECK(arr.capacity() == 9);
    CHECK(arr.empty() == false);
    CHECK(arr[2] == 2);
    arr.push_back(100);
    CHECK(arr.back() == 100);
    arr.pop_back();
    CHECK(arr.back() != 100);
    arr.clear();
    CHECK(arr.empty());
}
