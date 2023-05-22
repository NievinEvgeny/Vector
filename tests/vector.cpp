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
