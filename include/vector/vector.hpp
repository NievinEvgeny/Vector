#pragma once
#include <memory>
#include <utility>
#include <stdexcept>
#include <algorithm>

namespace vector {

constexpr std::size_t factor = 2;

template <typename T>
struct VecStorage
{
    std::size_t size = 0;
    std::size_t capacity = 1;
    T* data;

    constexpr VecStorage() : data(static_cast<T*>(operator new(sizeof(T))))
    {
    }

    constexpr VecStorage(std::size_t new_size, std::size_t new_capacity)
        : size(new_size), capacity(new_capacity), data(static_cast<T*>(operator new(sizeof(T) * new_capacity)))
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

    void push_back_internal(const T& value)
    {
        new (storage->data + storage->size) T(value);
        ++storage->size;
    }

    void move_back_internal(T&& value)
    {
        new (storage->data + storage->size) T(std::move(value));
        ++storage->size;
    }

    template <typename X>
    typename std::enable_if<!std::is_nothrow_move_constructible<X>::value>::type simple_copy(Vector<T>& dst)
    {
        std::for_each(storage->data, storage->data + storage->size, [&dst](T const& v) { dst.push_back_internal(v); });
    }

    template <typename X>
    typename std::enable_if<std::is_nothrow_move_constructible<X>::value>::type simple_copy(Vector<T>& dst)
    {
        std::for_each(
            storage->data, storage->data + storage->size, [&dst](T& v) { dst.move_back_internal(std::move(v)); });
    }

   public:
    constexpr Vector() : storage(std::make_shared<VecStorage<T>>())
    {
    }

    constexpr explicit Vector(std::size_t capacity)
        : storage(std::make_shared<VecStorage<T>>(VecStorage<T>{0, capacity}))
    {
    }

    constexpr void clear() noexcept
    {
        copy_storage();
        std::destroy_n(storage->data, size());
        storage->size = 0;
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

    constexpr void reserve(std::size_t new_capacity)
    {
        if (new_capacity > capacity())
        {
            Vector<T> tmp_buf(new_capacity);
            simple_copy<T>(tmp_buf);
            tmp_buf.swap(*this);
        }
    }

    constexpr void shrink_to_fit()
    {
        if ((size() == 0) || (size() == capacity()))
        {
            return;
        }

        Vector<T> tmp_buf(size());
        simple_copy<T>(tmp_buf);
        tmp_buf.swap(*this);
    }

    constexpr void push_back(const T& value)
    {
        copy_storage();

        if (size() == capacity())
        {
            reserve(storage->capacity * vector::factor);
        }

        push_back_internal(value);
    }

    constexpr void push_back(T&& value)
    {
        copy_storage();

        if (size() == capacity())
        {
            reserve(storage->capacity * vector::factor);
        }

        move_back_internal(std::move(value));
    }

    constexpr void pop_back()
    {
        copy_storage();
        --storage->size;
        std::destroy_at(&storage->data[size()]);
    }

    constexpr void resize(std::size_t count)
    {
        if (count == size())
        {
            return;
        }

        copy_storage();

        if (count < size())
        {
            std::destroy_n(storage->data + count, size() - 1 - count);
        }
        if (count >= capacity())
        {
            reserve(count * vector::factor);
        }
        if (count > size())
        {
            std::uninitialized_value_construct_n(storage->data + size(), count - size());
        }

        storage->size = count;
    }

    constexpr void resize(std::size_t count, const T& value)
    {
        if (count == size())
        {
            return;
        }

        copy_storage();

        if (count < size())
        {
            std::destroy_n(storage->data + count, size() - 1 - count);
        }
        if (count >= capacity())
        {
            reserve(count * vector::factor);
        }
        if (count > size())
        {
            std::uninitialized_fill_n(storage->data + size(), count - size(), value);
        }

        storage->size = count;
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

    constexpr iterator erase(const_iterator pos)
    {
        if (empty())
        {
            return end();
        }

        copy_storage();

        std::move(pos + 1, end(), pos);
        --storage->size;
        return pos;
    }

    constexpr iterator erase(const_iterator first, const_iterator last)
    {
        if (empty() || (last - first <= 0))
        {
            return end();
        }

        copy_storage();

        std::move(last, end(), first);
        storage->size -= last - first;
        return first;
    }
};

}  // namespace vector