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

    void copy_storage()
    {
        if (storage.use_count() != 1)
        {
            storage.reset(new VecStorage<T>(*storage.get()));
        }
    }

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

    constexpr T& at(std::size_t pos)
    {
        if (!(pos < size()))
        {
            throw std::out_of_range("Pos out of range");
        }
        copy_storage();
        return storage->data[pos];
    }

    constexpr const T& operator[](std::size_t pos) const
    {
        return storage->data[pos];
    }

    constexpr T& operator[](std::size_t pos)
    {
        copy_storage();
        return storage->data[pos];
    }

    class Iterator
    {
        Vector<T>* vector;
        std::size_t index;

       public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() : vector(nullptr), index(0)
        {
        }

        Iterator(Vector<T>* vector, std::size_t new_index) : vector(vector), index(new_index)
        {
        }

        reference operator*()
        {
            return vector->at(index);
        }
        const_reference operator*() const
        {
            return vector->at(index);
        }
        pointer operator->()
        {
            return &(vector->at(index));
        }
        const_pointer operator->() const
        {
            return &(vector->at(index));
        }
        reference operator[](std::size_t offset)
        {
            return vector->at(index + offset);
        }
        const_reference operator[](std::size_t offset) const
        {
            return vector->at(index + offset);
        }

        Iterator& operator++()
        {
            ++index;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator it(*this);
            ++index;
            return it;
        }
        Iterator& operator--()
        {
            --index;
            return *this;
        }
        Iterator operator--(int)
        {
            Iterator it(*this);
            --index;
            return it;
        }
        Iterator& operator+=(std::size_t offset)
        {
            index += offset;
            return *this;
        }
        Iterator& operator-=(std::size_t offset)
        {
            index -= offset;
            return *this;
        }
        Iterator operator+(std::size_t offset) const
        {
            Iterator it(*this);
            return it += offset;
        }
        friend Iterator operator+(std::size_t offset, Iterator it)
        {
            return it + offset;
        }
        Iterator operator-(std::size_t offset) const
        {
            Iterator it(*this);
            return it -= offset;
        }
        difference_type operator-(const Iterator& other) const
        {
            return index - other.index;
        }

        bool operator==(const Iterator& rhs) const
        {
            return index == rhs.index;
        }
        auto operator<=>(const Iterator& rhs) const
        {
            return index <=> rhs.index;
        }
    };

    using iterator = Iterator;
    using const_iterator = const Iterator;

    constexpr iterator begin() noexcept
    {
        return iterator(this, 0);
    }
    constexpr iterator end() noexcept
    {
        return iterator(this, size());
    }
    constexpr const_iterator begin() const noexcept
    {
        return const_iterator(const_cast<Vector<T>*>(this), 0);
    }
    constexpr const_iterator end() const noexcept
    {
        return const_iterator(const_cast<Vector<T>*>(this), size());
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }
    constexpr const_iterator cend() const noexcept
    {
        return end();
    }
};

}  // namespace vector