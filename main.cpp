#include <iostream> 

#include "DynamicArray.hpp"

#define PRINT(x) std::cout << x
#define NEWLINE std::cout << '\n'
#define PRINTLN(x) std::cout << x << '\n'

template<typename T>
void printArray(const DynamicArray<T>& arr)
{
    for (const auto& i : arr)
        std::cout << i << ' ';  
    std::cout << '\n';
}

int main(void)
{
    DynamicArray<int> arr;
    for (int i = 0; i < 20; i++)
        arr.pushBack(i);
    printArray(arr);
    PRINT(arr.getSize());
    NEWLINE;
    //arr.remove(19);
    //printArray(arr);
    PRINT(arr.front()); PRINT(arr.back());
    NEWLINE;
    PRINT(arr.at(2));
    NEWLINE;
    arr.at(2) = 22;
    printArray(arr);
    PRINT(arr.getCapacity());
    NEWLINE;
    PRINT(arr.getSize());
    NEWLINE;
    PRINT(arr.isEmpty());
    NEWLINE;
    arr.remove(2);
    printArray(arr);
    arr.insert(2, 2);
    printArray(arr);
    arr.popBack();
    printArray(arr);
    arr.emplaceBack(19);
    printArray(arr);
    arr.shrinkToFit();
    PRINT(arr.getSize());
    PRINT(arr.getCapacity());
    NEWLINE;
}