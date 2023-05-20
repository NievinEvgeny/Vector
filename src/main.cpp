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

    std::string test_str = "test";

    vector::Vector<std::string>::iterator iter_insert_copy = test4.insert(test4.begin(), test_str);
    std::cout << *iter_insert_copy << '\n';

    vector::Vector<std::string>::iterator iter_insert_move = test4.insert(test4.end(), std::move(test_str));
    std::cout << *iter_insert_move << '\n';

    vector::Vector<std::string>::iterator iter_insert_copy_n1 = test4.insert(test4.begin() + 10, 100, "elem");
    std::cout << *iter_insert_copy_n1 << '\n';

    vector::Vector<std::string>::iterator iter_insert_copy_n2 = test4.insert(test4.end(), 100, "elem");
    std::cout << *iter_insert_copy_n2 << '\n';

    test4.clear();

    return 0;
}
