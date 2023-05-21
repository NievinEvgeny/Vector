#include <vector/vector.hpp>
#include <iostream>
#include <string>

int main()
{
    vector::Vector<std::string> test;
    vector::Vector<std::string> test1 = test;
    vector::Vector<std::string> test2 = std::move(test1);
    vector::Vector<std::string> test3{std::move(test2)};
    vector::Vector<std::string> test4{test3};

    test4.reserve(200);

    for (int i = 0; i < 10; i++)
    {
        test4.push_back(std::to_string(i));
    }

    test4.shrink_to_fit();

    vector::Vector<std::string>::iterator iter_insert
        = test4.insert(test4.begin() + 2, test4.cbegin() + 4, test4.cend());
    std::cout << *iter_insert << '\n';

    test4.clear();

    return 0;
}
