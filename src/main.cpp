#include <vector/vector.hpp>
#include <iostream>

int main()
{
    vector::Vector<int> test;
    vector::Vector<int> test1 = test;
    vector::Vector<int> test2 = std::move(test1);
    vector::Vector<int> test3{std::move(test2)};
    vector::Vector<int> test4{test3};

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';

    test4.reserve(10);

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';

    test4.shrink_to_fit();

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';

    test4.clear();

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';
    return 0;
}
