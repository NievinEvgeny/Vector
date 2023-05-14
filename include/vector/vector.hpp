#pragma once
#include <memory>
#include <utility>
#include <stdexcept>

namespace vector {

template <typename T>
struct VecStorage
{
    std::size_t size = 0;
    std::size_t capacity = 1;
    T* data;

    constexpr VecStorage() : data(static_cast<T*>(operator new(sizeof(T))))
    {
    }

    constexpr ~VecStorage()
    {
        std::destroy_n(data, size);
        operator delete(data);
    }

    constexpr void swap(VecStorage<T>& other) noexcept
    {
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
        std::swap(data, other.data);
    }

    constexpr VecStorage(const VecStorage<T>& copy)
        : size(copy.size), capacity(copy.capacity), data(static_cast<T*>(operator new(sizeof(T) * copy.capacity)))
    {
        std::uninitialized_copy_n(copy.data, copy.size, data);
    }

    constexpr VecStorage& operator=(VecStorage<T> other) noexcept
    {
        other.swap(*this);
        return *this;
    }

    constexpr VecStorage(VecStorage<T>&& other) noexcept
        : size(std::move(other.size)), capacity(std::move(other.capacity)), data(std::exchange(other.data, nullptr))
    {
    }
};

template <typename T>
class Vector
{
    std::shared_ptr<VecStorage<T>> storage;

   public:
    constexpr Vector() : storage(std::make_shared<VecStorage<T>>())
    {
    }

    constexpr void swap(Vector<T>& other) noexcept
    {
        std::swap(storage, other.storage);
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept
    {
        return storage->size;
    }

    [[nodiscard]] constexpr std::size_t capacity() const noexcept
    {
        return storage->capacity;
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return storage->size == 0;
    }

    constexpr const T& at(std::size_t pos) const
    {
        if (!(pos < size()))
        {
            throw std::out_of_range("Pos out of range");
        }
        return storage->data[pos];
    }

    constexpr const T& operator[](std::size_t pos) const
    {
        return storage->data[pos];
    }
};

}  // namespace vector