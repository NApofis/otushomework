//
// Created by ntikhonov on 20.07.24.
// 1. Cобственный STL-совместимый аллокатор.
// 1.2 Использование его в стандартных контейнерах.
// 2. Свой контейнер, поддерживающий параметризацию аллокатором по аналогии с STL.
//


#include "container.h"
#include "allocator.h"

#include <iostream>
#include <map>


long double fact(int N)
{
    if(N < 0)
        return 0;
    if (N == 0)
        return 1;
    return N * fact(N - 1);
}

int main() {

    std::map<int, int> map_1;
    for (auto i = 0; i < 10; i++)
    {
        map_1[i] = fact(i);
    }

    std::map<int, int, std::less<int>, update_allocator<std::pair<const int, int>>> map_2;
    for (auto i = 0; i < 10; i++)
    {
        map_2[i] = fact(i);
    }
    for(auto [key, value] : map_2)
    {
        std::cout << key << " " << value << std::endl;
    }

    Deque<int> my;
    for (auto i = 0; i < 10; i++)
    {
        my.emplace(i);
    }

    Deque<int, update_allocator<int>> my_2;
    for (auto i = 0; i < 10; i++)
    {
        my_2.emplace(i);
    }
    for(auto i : my_2)
    {
        std::cout << i << std::endl;
    }
    return 0;
}