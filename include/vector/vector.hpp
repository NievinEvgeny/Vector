#pragma once
#include <memory>
#include <utility>

namespace vector {

template <typename T>
struct VecStorage
{
    std::size_t size = 0;
    std::size_t capacity = 1;
    T* data;

    VecStorage() : data(static_cast<T*>(operator new(sizeof(T))))
    {
    }

    ~VecStorage()
    {
        std::destroy_n(data, size);
        operator delete(data);
    }

    void swap(VecStorage<T>& other) noexcept
    {
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
        std::swap(data, other.data);
    }

    VecStorage(const VecStorage<T>& copy)
        : size(copy.size), capacity(copy.capacity), data(static_cast<T*>(operator new(sizeof(T) * copy.capacity)))
    {
        std::uninitialized_copy_n(copy.data, copy.size, data);
    }

    VecStorage& operator=(VecStorage<T> other) noexcept
    {
        other.swap(*this);
        return *this;
    }

    VecStorage(VecStorage<T>&& other) noexcept
        : size(std::move(other.size)), capacity(std::move(other.capacity)), data(std::exchange(other.data, nullptr))
    {
    }
};

}  // namespace vector