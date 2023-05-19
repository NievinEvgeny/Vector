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

    for (int i = 0; i < 100; i++)
    {
        test4.push_back(std::to_string(i));
    }

    test4.shrink_to_fit();

    std::string test_str = "test";
    std::string test_str_n = "elem";

    vector::Vector<std::string>::iterator iter_erase = test4.erase(test4.begin() + 3, test4.begin() + 10);
    std::cout << *iter_erase << '\n';

    vector::Vector<std::string>::iterator iter_insert_copy = test4.insert(test4.begin() + 3, test_str);
    std::cout << *iter_insert_copy << '\n';

    vector::Vector<std::string>::iterator iter_insert_move = test4.insert(test4.end(), std::move(test_str_n));
    std::cout << *iter_insert_move << '\n';

    test4.clear();

    return 0;
}
