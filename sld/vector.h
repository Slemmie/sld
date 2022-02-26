// -------------------------
//  dynamically sized array
// -------------------------

#pragma once

#ifndef _SLD_VECTOR_H
#define _SLD_VECTOR_H

#include <sld/core.h>
#include <sld/types.h>
#include <sld/memory.h>
#include <sld/exception.h>
#include <sld/algorithm.h>
#include <sld/utility.h>

_SLD_BEGIN

template <class A> class vector {
	
public:
	
	typedef A* iterator;
	
	constexpr vector() noexcept :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{ }
	
	vector(uint64 _size) :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{
		this->resize(_size);
	}
	
	vector(uint64 _size, const A& _initial) :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{
		this->resize(_size, _initial);
	}
	
	vector(const vector <A>& _vector) noexcept :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{
		this->assign(_vector);
	}
	
	~vector() {
		this->clear();
	}
	
	inline void clear() noexcept {
		if (this->m_data) {
			for (_SLD vector <A>::iterator it = this->begin(); it != this->end(); ++it) {
				it->~A();
			}
			_SLD free(this->m_data);
			this->m_data = nullptr;
			this->m_size = this->m_capacity = 0;
		}
	}
	
	inline void reserve(uint64 _capacity) noexcept {
		if (_capacity <= this->capacity()) {
			return;
		}
		uint64 new_capacity = _SLD max(uint64(1), this->capacity());
		while (new_capacity < _capacity) {
			new_capacity <<= 1;
		}
		A* new_data = (A*) _SLD malloc(sizeof(A) * new_capacity);
		_SLD memcpy(new_data, this->m_data, sizeof(A) * this->size());
		_SLD free(this->m_data);
		this->m_data = new_data;
		this->m_capacity = new_capacity;
	}
	
	inline void resize(uint64 _size) {
		if (_size < this->size()) {
			while (_size < this->size()) {
				this->pop_back();
			}
			return;
		}
		if (_size == this->size()) {
			return;
		}
		this->reserve(_size);
		uint64 old_size = this->size();
		this->m_size = _size;
		for (_SLD vector <A>::iterator it = this->begin() + old_size; it != this->end(); ++it) {
			new (it) A();
		}
	}
	
	inline void resize(uint64 _size, const A& _initial) {
		if (_size < this->size()) {
			while (_size < this->size()) {
				this->pop_back();
			}
			return;
		}
		if (_size == this->size()) {
			return;
		}
		this->reserve(_size);
		uint64 old_size = this->size();
		this->m_size = _size;
		_SLD fill(this->begin() + old_size, this->end(), _initial);
	}
	
	inline void push_back(const A& _value) noexcept {
		this->reserve(this->size() + 1);
		this->m_data[this->m_size++] = _value;
	}
	
	template <typename... ARGS> inline void emplace_back(ARGS&&... _args) {
		this->reserve(this->size() + 1);
		new (&this->m_data[this->m_size++]) A(_args...);
	}
	
	inline void pop_back() {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		(&this->m_data[--this->m_size])->~A();
	}
	
	inline void insert(_SLD vector <A>::iterator _ptr, const A& _value) {
		uint64 index_before_resize = _ptr - this->begin();
		this->push_back(_value);
		for (_SLD vector <A>::iterator it = this->end(); it != _ptr + 1; --it) {
			_SLD swap(*(it - 1), *(it - 2));
		}
	}
	
	template <typename... ARGS> inline void emplace(_SLD vector <A>::iterator _ptr, ARGS&&... _args) {
		uint64 index_before_resize = _ptr - this->begin();
		this->emplace_back(_args...);
		for (_SLD vector <A>::iterator it = this->end(); it != _ptr + 1; --it) {
			_SLD swap(*(it - 1), *(it - 2));
		}
	}
	
	inline void insert(_SLD vector <A>::iterator _ptr,
	const _SLD vector <A>::iterator _begin, const _SLD vector <A>::iterator _end) {
		uint64 required_size = _end - _begin;
		uint64 index_before_resize = _ptr - this->begin();
		this->resize(this->size() + required_size);
		_ptr = this->begin() + index_before_resize;
		for (_SLD vector <A>::iterator it = this->end(); it != _ptr + required_size; --it) {
			_SLD swap(*(it - 1), *(it - required_size - 1));
		}
		_SLD memcpy(_ptr, _begin, sizeof(A) * required_size);
	}
	
	inline void insert(_SLD vector <A>::iterator _ptr, uint64 _count, const A& _value) {
		uint64 index_before_resize = _ptr - this->begin();
		this->resize(this->size() + _count);
		_ptr = this->begin() + index_before_resize;
		for (_SLD vector <A>::iterator it = this->end() - _count - 1; it + 1 != _ptr; --it) {
			_SLD swap(*it, *(it + _count));
		}
		for (_SLD vector <A>::iterator it = _ptr; it != _ptr + _count; ++it) {
			*it = A(_value);
		}
	}
	
	inline void erase(_SLD vector <A>::iterator _ptr) {
		if (_ptr < this->begin() || _ptr >= this->end()) {
			this->m_throw_bounds();
		}
		if (_ptr + 1 == this->end()) {
			this->pop_back();
			return;
		}
		for (_SLD vector <A>::iterator it = _ptr + 1; it + 1 != this->end(); ++it) {
			_SLD swap(*it, *(it + 1));
		}
		this->pop_back();
	}
	
	inline void erase(_SLD vector <A>::iterator _begin, _SLD vector <A>::iterator _end) {
		if (_begin < this->begin() || _end > this->end() || _begin > _end) {
			this->m_throw_bounds();
		}
		uint64 required_size = _end - _begin;
		for (_SLD vector <A>::iterator it = _end; it != this->end(); ++it) {
			_SLD swap(*it, *(it - required_size));
		}
		for (uint64 i = 0; i < required_size; i++) {
			this->pop_back();
		}
	}
	
	inline void assign(const _SLD vector <A>& _vector) noexcept {
		this->clear();
		this->m_size = _vector.size();
		this->m_capacity = _vector.capacity();
		this->m_data = (A*) _SLD malloc(this->capacity());
		_SLD memcpy(this->m_data, _vector.begin(), sizeof(A) * _vector.size());
	}
	
	inline _SLD vector <A>& operator = (const _SLD vector <A>& _vector) noexcept {
		this->assign(_vector);
		return *this;
	}
	
	constexpr const A& operator [] (const uint64 _index) const noexcept {
		return this->m_data[_index];
	}
	
	inline A& operator [] (const uint64 _index) noexcept {
		return this->m_data[_index];
	}
	
	constexpr const A& at(const uint64 _index) const {
		if (_index >= this->size()) {
			this->m_throw_bounds();
		}
		return this->m_data[_index];
	}
	
	inline A& at(const uint64 _index) {
		if (_index >= this->size()) {
			this->m_throw_bounds();
		}
		return this->m_data[_index];
	}
	
	constexpr const A& front() const {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[0];
	}
	
	inline A& front() {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[0];
	}
	
	constexpr const A& back() const {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[this->size() - 1];
	}
	
	inline A& back() {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[this->size() - 1];
	}
	
	constexpr const _SLD vector <A>::iterator begin() const noexcept {
		return this->m_data;
	}
	
	inline _SLD vector <A>::iterator begin() noexcept {
		return this->m_data;
	}
	
	constexpr const _SLD vector <A>::iterator end() const noexcept {
		return this->m_data + this->size();
	}
	
	inline _SLD vector <A>::iterator end() noexcept {
		return this->m_data + this->size();
	}
	
	constexpr uint64 size() const noexcept {
		return this->m_size;
	}
	
	constexpr bool empty() const noexcept {
		return !(bool)this->m_size;
	}
	
	constexpr uint64 capacity() const noexcept {
		return this->m_capacity;
	}
	
private:
	
	A* m_data;
	
	uint64 m_size;
	uint64 m_capacity;
	
private:
	
	constexpr void m_throw_bounds() {
		throw _SLD except::out_of_bounds("out of bounds sld::vector subscript");
	}
	
};

_SLD_END

#endif /// _SLD_VECTOR_H
