#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>

namespace sjtu
{
template<typename T>
class vector
{
public:
	class const_iterator;
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		T* ptr;
		const vector* vec;

	public:
		iterator() : ptr(nullptr), vec(nullptr) {}
		iterator(T* p, const vector* v) : ptr(p), vec(v) {}

		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, vec);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, vec);
		}
		int operator-(const iterator &rhs) const
		{
			if (vec != rhs.vec) {
				throw invalid_iterator();
			}
			return static_cast<int>(ptr - rhs.ptr);
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		iterator& operator++()
		{
			++ptr;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		iterator& operator--()
		{
			--ptr;
			return *this;
		}
		T& operator*() const
		{
			return *ptr;
		}
		T* operator->() const
		{
			return ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator<(const iterator &rhs) const
		{
			return ptr < rhs.ptr;
		}
		bool operator>(const iterator &rhs) const
		{
			return ptr > rhs.ptr;
		}
		bool operator<=(const iterator &rhs) const
		{
			return ptr <= rhs.ptr;
		}
		bool operator>=(const iterator &rhs) const
		{
			return ptr >= rhs.ptr;
		}

		friend class vector;
		friend class const_iterator;
	};

	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		const T* ptr;
		const vector* vec;

	public:
		const_iterator() : ptr(nullptr), vec(nullptr) {}
		const_iterator(const T* p, const vector* v) : ptr(p), vec(v) {}
		const_iterator(const iterator &it) : ptr(it.ptr), vec(it.vec) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, vec);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, vec);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (vec != rhs.vec) {
				throw invalid_iterator();
			}
			return static_cast<int>(ptr - rhs.ptr);
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		const_iterator& operator++()
		{
			++ptr;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		const_iterator& operator--()
		{
			--ptr;
			return *this;
		}
		const T& operator*() const
		{
			return *ptr;
		}
		const T* operator->() const
		{
			return ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator<(const const_iterator &rhs) const
		{
			return ptr < rhs.ptr;
		}
		bool operator>(const const_iterator &rhs) const
		{
			return ptr > rhs.ptr;
		}
		bool operator<=(const const_iterator &rhs) const
		{
			return ptr <= rhs.ptr;
		}
		bool operator>=(const const_iterator &rhs) const
		{
			return ptr >= rhs.ptr;
		}

		friend class vector;
		friend class iterator;
	};

private:
	T* data;
	size_t _size;
	size_t _capacity;

	void reallocate(size_t new_capacity)
	{
		if (new_capacity == 0) {
			new_capacity = 1;
		}
		T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
		for (size_t i = 0; i < _size; ++i) {
			new (new_data + i) T(data[i]);
		}
		for (size_t i = 0; i < _size; ++i) {
			(data + i)->~T();
		}
		::operator delete(data);
		data = new_data;
		_capacity = new_capacity;
	}

	void expand()
	{
		size_t new_capacity = _capacity * 2;
		if (new_capacity == 0) {
			new_capacity = 1;
		}
		reallocate(new_capacity);
	}

public:
	vector() : data(nullptr), _size(0), _capacity(0) {}

	vector(const vector &other) : data(nullptr), _size(0), _capacity(0)
	{
		if (other._size > 0) {
			data = static_cast<T*>(::operator new(other._size * sizeof(T)));
			_capacity = other._size;
			for (size_t i = 0; i < other._size; ++i) {
				new (data + i) T(other.data[i]);
			}
			_size = other._size;
		}
	}

	~vector()
	{
		clear();
		::operator delete(data);
		data = nullptr;
		_capacity = 0;
	}

	vector &operator=(const vector &other)
	{
		if (this == &other) {
			return *this;
		}
		clear();
		if (other._size > 0) {
			if (_capacity < other._size) {
				::operator delete(data);
				data = static_cast<T*>(::operator new(other._size * sizeof(T)));
				_capacity = other._size;
			}
			for (size_t i = 0; i < other._size; ++i) {
				new (data + i) T(other.data[i]);
			}
			_size = other._size;
		}
		return *this;
	}

	T & at(const size_t &pos)
	{
		if (pos >= _size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	const T & at(const size_t &pos) const
	{
		if (pos >= _size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	T & operator[](const size_t &pos)
	{
		if (pos >= _size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	const T & operator[](const size_t &pos) const
	{
		if (pos >= _size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	const T & front() const
	{
		if (_size == 0) {
			throw container_is_empty();
		}
		return data[0];
	}

	const T & back() const
	{
		if (_size == 0) {
			throw container_is_empty();
		}
		return data[_size - 1];
	}

	iterator begin()
	{
		return iterator(data, this);
	}

	const_iterator begin() const
	{
		return const_iterator(data, this);
	}

	const_iterator cbegin() const
	{
		return const_iterator(data, this);
	}

	iterator end()
	{
		return iterator(data + _size, this);
	}

	const_iterator end() const
	{
		return const_iterator(data + _size, this);
	}

	const_iterator cend() const
	{
		return const_iterator(data + _size, this);
	}

	bool empty() const
	{
		return _size == 0;
	}

	size_t size() const
	{
		return _size;
	}

	void clear()
	{
		for (size_t i = 0; i < _size; ++i) {
			(data + i)->~T();
		}
		_size = 0;
	}

	iterator insert(iterator pos, const T &value)
	{
		size_t idx = pos.ptr - data;
		return insert(idx, value);
	}

	iterator insert(const size_t &ind, const T &value)
	{
		if (ind > _size) {
			throw index_out_of_bound();
		}
		if (_size == _capacity) {
			expand();
		}
		for (size_t i = _size; i > ind; --i) {
			new (data + i) T(data[i - 1]);
			(data + i - 1)->~T();
		}
		new (data + ind) T(value);
		++_size;
		return iterator(data + ind, this);
	}

	iterator erase(iterator pos)
	{
		size_t idx = pos.ptr - data;
		return erase(idx);
	}

	iterator erase(const size_t &ind)
	{
		if (ind >= _size) {
			throw index_out_of_bound();
		}
		(data + ind)->~T();
		for (size_t i = ind; i < _size - 1; ++i) {
			new (data + i) T(data[i + 1]);
			(data + i + 1)->~T();
		}
		--_size;
		return iterator(data + ind, this);
	}

	void push_back(const T &value)
	{
		if (_size == _capacity) {
			expand();
		}
		new (data + _size) T(value);
		++_size;
	}

	void pop_back()
	{
		if (_size == 0) {
			throw container_is_empty();
		}
		--_size;
		(data + _size)->~T();
	}
};

}

#endif
