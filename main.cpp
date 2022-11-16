#include <iostream> 

#include "MyVector.hpp"

#define PRINT(x) std::cout << x
#define NEWLINE std::cout << '\n'
#define PRINTLN(x) std::cout << x << '\n'

template<typename T>
void printArray(const MyVector<T>& arr)
{
    for (const auto& i : arr)
        std::cout << i << ' ';  
    std::cout << '\n';
}

int main(void)
{
    MyVector<int> arr;
    for (int i = 0; i < 20; i++)
        arr.push_back(i);
    printArray(arr);
    PRINT(arr.size());
    NEWLINE;
    //arr.remove(19);
    //printArray(arr);
    PRINT(arr.front()); PRINT(arr.back());
    NEWLINE;
    PRINT(arr.at(2));
    NEWLINE;
    arr.at(2) = 22;
    printArray(arr);
    PRINT(arr.capacity());
    NEWLINE;
    PRINT(arr.size());
    NEWLINE;
    PRINT(std::boolalpha << arr.empty());
    NEWLINE;
    arr.remove(2);
    printArray(arr);
    arr.insert(2, 2);
    printArray(arr);
    arr.pop_back();
    printArray(arr);
    arr.emplace_back(19);
    printArray(arr);
    arr.shrinkToFit();
    PRINT(arr.size());
    PRINT(arr.capacity());
    NEWLINE;
}
