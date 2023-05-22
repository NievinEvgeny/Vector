#include <vector/vector.hpp>
#include <gtest/gtest.h>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

TEST(VecStorage, DefaultConstructor)
{
    constexpr std::size_t exp_size = 0;
    constexpr std::size_t exp_capacity = 1;
    vector::VecStorage<std::string> storage;

    EXPECT_EQ(storage.size, exp_size);
    EXPECT_EQ(storage.capacity, exp_capacity);
}

TEST(VecStorage, ConstructorWithCapacity)
{
    constexpr std::size_t exp_size = 0;
    constexpr std::size_t exp_capacity = 5;
    vector::VecStorage<std::string> storage{exp_capacity};

    EXPECT_EQ(storage.size, exp_size);
    EXPECT_EQ(storage.capacity, exp_capacity);
}

TEST(VecStorage, Swap)
{
    const std::string first_data = "first";
    constexpr std::size_t first_capacity = 5;
    vector::VecStorage<std::string> first_storage{first_capacity};
    new (first_storage.data) std::string(first_data);

    const std::string sec_data = "second";
    constexpr std::size_t sec_capacity = 3;
    vector::VecStorage<std::string> sec_storage{sec_capacity};
    new (sec_storage.data) std::string(sec_data);

    first_storage.swap(sec_storage);

    EXPECT_EQ(first_storage.capacity, sec_capacity);
    EXPECT_EQ(*first_storage.data, sec_data);

    EXPECT_EQ(sec_storage.capacity, first_capacity);
    EXPECT_EQ(*sec_storage.data, first_data);
}

TEST(VecStorage, CopyConstructor)
{
    const std::string orig_data = "copy";
    constexpr std::size_t orig_capacity = 5;
    constexpr std::size_t orig_size = 1;
    vector::VecStorage<std::string> original{orig_capacity};
    original.size = orig_size;
    new (original.data) std::string(orig_data);

    vector::VecStorage<std::string> copy{original};

    EXPECT_EQ(copy.capacity, original.capacity);
    EXPECT_EQ(*copy.data, *original.data);
}

TEST(VecStorage, CopyAssignment)
{
    const std::string orig_data = "copy";
    constexpr std::size_t orig_capacity = 5;
    constexpr std::size_t orig_size = 1;
    vector::VecStorage<std::string> original{orig_capacity};
    original.size = orig_size;
    new (original.data) std::string(orig_data);

    vector::VecStorage<std::string> copy = original;

    EXPECT_EQ(copy.capacity, original.capacity);
    EXPECT_EQ(*copy.data, *original.data);
}

TEST(VecStorage, MoveConstructor)
{
    const std::string orig_data = "move";
    constexpr std::size_t orig_capacity = 5;
    constexpr std::size_t orig_size = 1;
    vector::VecStorage<std::string> original{orig_capacity};
    original.size = orig_size;
    new (original.data) std::string(orig_data);

    vector::VecStorage<std::string> move{std::move(original)};

    EXPECT_EQ(move.capacity, orig_capacity);
    EXPECT_EQ(*move.data, orig_data);
}

TEST(VecStorage, MoveAssignment)
{
    const std::string orig_data = "move";
    constexpr std::size_t orig_capacity = 5;
    constexpr std::size_t orig_size = 1;
    vector::VecStorage<std::string> original{orig_capacity};
    original.size = orig_size;
    new (original.data) std::string(orig_data);

    vector::VecStorage<std::string> move = std::move(original);

    EXPECT_EQ(move.capacity, orig_capacity);
    EXPECT_EQ(*move.data, orig_data);
}

TEST(Vector, Reserve)
{
    constexpr std::size_t exp_size = 3;
    constexpr std::size_t exp_capacity = 30;
    vector::Vector<std::string> vec;

    const std::vector<std::string> values{"val1", "val2", "val3"};

    for (const auto& val : values)
    {
        vec.push_back(val);
    }

    vec.reserve(exp_capacity);

    EXPECT_EQ(vec.size(), exp_size);
    EXPECT_EQ(vec.capacity(), exp_capacity);

    for (std::size_t i = 0; i < values.size(); i++)
    {
        EXPECT_EQ(vec[i], values.at(i));
    }
}

TEST(Vector, ShrinkToFit)
{
    constexpr std::size_t reserve_capacity = 100;
    constexpr std::size_t exp_size = 3;
    constexpr std::size_t exp_capacity = 3;
    vector::Vector<std::string> vec;

    const std::vector<std::string> values{"val1", "val2", "val3"};

    vec.reserve(reserve_capacity);

    for (const auto& val : values)
    {
        vec.push_back(val);
    }

    vec.shrink_to_fit();

    EXPECT_EQ(vec.size(), exp_size);
    EXPECT_EQ(vec.capacity(), exp_capacity);

    for (std::size_t i = 0; i < values.size(); i++)
    {
        EXPECT_EQ(vec[i], values.at(i));
    }
}

TEST(Vector, PushBackCopy)
{
    constexpr std::size_t exp_size = 3;
    constexpr std::size_t exp_capacity = 4;
    vector::Vector<std::string> vec;

    const std::vector<std::string> values{"val1", "val2", "val3"};

    for (const auto& val : values)
    {
        vec.push_back(val);
    }

    EXPECT_EQ(vec.size(), exp_size);
    EXPECT_EQ(vec.capacity(), exp_capacity);

    for (std::size_t i = 0; i < values.size(); i++)
    {
        EXPECT_EQ(vec[i], values.at(i));
    }
}

TEST(Vector, PushBackMove)
{
    constexpr std::size_t exp_size = 3;
    constexpr std::size_t exp_capacity = 4;
    vector::Vector<std::string> vec;

    const std::vector<std::string> exp_values{"val1", "val2", "val3"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(std::move(val));
    }

    EXPECT_EQ(vec.size(), exp_size);
    EXPECT_EQ(vec.capacity(), exp_capacity);

    for (std::size_t i = 0; i < exp_values.size(); i++)
    {
        EXPECT_EQ(vec[i], exp_values.at(i));
    }
}

TEST(Vector, AtOutOfBounds)
{
    constexpr std::size_t index_out_of_range = 10;
    vector::Vector<std::string> vec;

    EXPECT_THROW(vec.at(index_out_of_range), std::out_of_range);
}

TEST(Vector, PopBack)
{
    vector::Vector<std::string> vec;

    const std::vector<std::string> exp_values{"val1", "val2", "val3"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    const std::size_t invalid_index = values.size() - 1;

    for (auto& val : values)
    {
        vec.push_back(std::move(val));
    }

    vec.pop_back();

    EXPECT_THROW(vec.at(invalid_index), std::out_of_range);
}

TEST(Vector, Clear)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(std::move(val));
    }

    vec.clear();

    EXPECT_THROW(vec.at(0), std::out_of_range);
}

TEST(Vector, Swap)
{
    vector::Vector<std::string> vec1;
    std::vector<std::string> values1{"1", "1", "1"};

    for (auto& val : values1)
    {
        vec1.push_back(val);
    }

    vector::Vector<std::string> vec2;
    std::vector<std::string> values2{"2", "2", "2", "2", "2"};

    for (auto& val : values2)
    {
        vec2.push_back(val);
    }

    vec1.swap(vec2);

    EXPECT_EQ(vec1.size(), values2.size());
    EXPECT_EQ(vec2.size(), values1.size());

    for (const auto& num1 : vec1)
    {
        EXPECT_EQ(num1, values2.at(0));
    }

    for (const auto& num2 : vec2)
    {
        EXPECT_EQ(num2, values1.at(0));
    }
}

TEST(Vector, DefaultResizeCountBelowSize)
{
    constexpr std::size_t new_size = 1;
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(std::move(val));
    }

    vec.resize(new_size);

    EXPECT_THROW(vec.at(2), std::out_of_range);
}

TEST(Vector, DefaultResizeCountAboveSize)
{
    constexpr std::size_t exp_size = 5;
    constexpr std::size_t new_size = 5;
    constexpr std::size_t new_index = 4;
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(std::move(val));
    }

    vec.resize(new_size);

    EXPECT_EQ(vec.size(), exp_size);
    EXPECT_NO_THROW(vec.at(new_index));
}

TEST(Vector, FillResizeCountAboveSize)
{
    constexpr std::size_t exp_size = 10;
    constexpr std::size_t new_size = 10;
    constexpr std::size_t new_index = 6;
    const std::string fill_value = "new value";
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(std::move(val));
    }

    vec.resize(new_size, fill_value);

    EXPECT_EQ(vec.size(), exp_size);
    EXPECT_EQ(vec.at(new_index), fill_value);
}

TEST(Iterator, ElementAccess)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter = vec.begin();

    EXPECT_EQ(*iter, values.at(0));
    EXPECT_EQ(iter[1], values.at(1));
    EXPECT_EQ(iter->npos, std::string::npos);
}

TEST(Iterator, IncrementAndDecrement)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter = vec.begin();
    EXPECT_EQ(*(iter++), values.at(0));
    EXPECT_EQ(*(++iter), values.at(2));
    EXPECT_EQ(*(iter--), values.at(2));
    EXPECT_EQ(*(--iter), values.at(0));
}

TEST(Iterator, ShiftByOffset)
{
    constexpr std::size_t offset = 2;
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter = vec.begin();
    iter += offset;
    EXPECT_EQ(*iter, values.at(2));
    iter -= offset;
    EXPECT_EQ(*iter, values.at(0));
}

TEST(Iterator, AddAndSubWithOffset)
{
    constexpr std::size_t offset = 2;
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter_begin = vec.begin();
    vector::Vector<std::string>::iterator iter_end = vec.end() - 1;
    EXPECT_EQ(*(iter_begin + offset), values.at(2));
    EXPECT_EQ(*(offset + iter_begin), values.at(2));
    EXPECT_EQ(*(iter_end - offset), values.at(0));
}

TEST(Iterator, SubIters)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    EXPECT_EQ(vec.end() - vec.begin(), vec.size());
}

TEST(Iterator, Comparison)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter_begin = vec.begin();
    vector::Vector<std::string>::iterator iter_end = vec.end();

    EXPECT_TRUE(iter_end > iter_begin);
    EXPECT_TRUE(iter_end != iter_begin);
    EXPECT_TRUE(iter_end - vec.size() == iter_begin);
}

TEST(Vector, EraseSingleElement)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val3", "val4", "val5", "val6"};
    std::vector<std::string> values{"val1", "val2", "val3", "val4", "val5", "val6"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.erase(vec.begin() + 1);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, EraseRangeOfElements)
{
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val5", "val6"};
    std::vector<std::string> values{"val1", "val2", "val3", "val4", "val5", "val6"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.erase(vec.begin() + 1, vec.begin() + 4);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertElementCopy)
{
    const std::string value = "insert";
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "insert", "val2", "val3"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.insert(vec.begin() + 1, value);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertElementCopyToEnd)
{
    const std::string value = "insert";
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val2", "val3", "insert"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.insert(vec.end(), value);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertElementMove)
{
    std::string value = "insert";
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "insert", "val2", "val3"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.insert(vec.begin() + 1, std::move(value));

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertElementMoveToEnd)
{
    std::string value = "insert";
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val2", "val3", "insert"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.insert(vec.end(), std::move(value));

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertCountElements)
{
    constexpr std::size_t count = 3;
    std::string value = "insert";
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "insert", "insert", "insert", "val2", "val3"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.insert(vec.begin() + 1, count, value);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertCountElementsToEnd)
{
    constexpr std::size_t count = 3;
    std::string value = "insert";
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val2", "val3", "insert", "insert", "insert"};
    std::vector<std::string> values{"val1", "val2", "val3"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vec.insert(vec.end(), count, value);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertRangeElements)
{
    constexpr std::size_t range_begin = 3;
    constexpr std::size_t range_end = 5;
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val4", "val5", "val2", "val3", "val4", "val5", "val6", "val7", "val8"};
    std::vector<std::string> values{"val1", "val2", "val3", "val4", "val5", "val6", "val7", "val8"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter_begin = vec.begin() + range_begin;
    vector::Vector<std::string>::iterator iter_end = vec.begin() + range_end;

    vec.insert(vec.begin() + 1, iter_begin, iter_end);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}

TEST(Vector, InsertRangeElementsToEnd)
{
    constexpr std::size_t range_begin = 3;
    constexpr std::size_t range_end = 5;
    vector::Vector<std::string> vec;

    std::vector<std::string> exp_values{"val1", "val2", "val3", "val4", "val5", "val6", "val7", "val8", "val4", "val5"};
    std::vector<std::string> values{"val1", "val2", "val3", "val4", "val5", "val6", "val7", "val8"};

    for (auto& val : values)
    {
        vec.push_back(val);
    }

    vector::Vector<std::string>::iterator iter_begin = vec.begin() + range_begin;
    vector::Vector<std::string>::iterator iter_end = vec.begin() + range_end;

    vec.insert(vec.end(), iter_begin, iter_end);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        EXPECT_EQ(vec.at(i), exp_values.at(i));
    }
}
