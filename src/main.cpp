#include <vector/vector.hpp>
#include <iostream>

int main()
{
    vector::VecStorage<int> test;
    vector::VecStorage<int> test1 = test;
    vector::VecStorage<int> test2 = std::move(test1);
    vector::VecStorage<int> test3{std::move(test2)};
    vector::VecStorage<int> test4{test3};
    return 0;
}
