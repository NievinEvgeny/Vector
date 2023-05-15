#include <vector/vector.hpp>
#include <iostream>

int main()
{
    vector::Vector<int> test;
    vector::Vector<int> test1 = test;
    vector::Vector<int> test2 = std::move(test1);
    vector::Vector<int> test3{std::move(test2)};
    vector::Vector<int> test4{test3};

    for (int i = 0; i < 100; i++)
    {
        test4.push_back(i);
    }

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';

    test4.reserve(200);

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';

    test4.shrink_to_fit();

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';

    test4.clear();

    std::cout << test4.size() << ' ' << test4.capacity() << '\n';
    return 0;
}
